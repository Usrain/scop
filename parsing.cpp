#include "includes/scop.hpp"
#include "fstream"
#include <cstdint>
#include <sstream>
#include <map>
#include <tuple>

static void computeNormalsIfMissing(std::vector<Vertex>& out_vertices,
                                     const std::vector<uint32_t>& out_indices,
                                     bool hadNormals)
{
    if (hadNormals)
        return;
    for (size_t i = 0; i + 2 < out_indices.size(); i += 3)
    {
        Vertex& v0 = out_vertices[out_indices[i]];
        Vertex& v1 = out_vertices[out_indices[i + 1]];
        Vertex& v2 = out_vertices[out_indices[i + 2]];

        Vec3 edge1 = { v1.position.x - v0.position.x, v1.position.y - v0.position.y, v1.position.z - v0.position.z };
        Vec3 edge2 = { v2.position.x - v0.position.x, v2.position.y - v0.position.y, v2.position.z - v0.position.z };

        Vec3 faceNormal = {
            edge2.y * edge1.z - edge2.z * edge1.y,
            edge2.z * edge1.x - edge2.x * edge1.z,
            edge2.x * edge1.y - edge2.y * edge1.x
        };

        v0.normal.x += faceNormal.x; v0.normal.y += faceNormal.y; v0.normal.z += faceNormal.z;
        v1.normal.x += faceNormal.x; v1.normal.y += faceNormal.y; v1.normal.z += faceNormal.z;
        v2.normal.x += faceNormal.x; v2.normal.y += faceNormal.y; v2.normal.z += faceNormal.z;
    }
    for (Vertex& v : out_vertices)
    {
        float len = std::sqrt(v.normal.x * v.normal.x + v.normal.y * v.normal.y + v.normal.z * v.normal.z);
        if (len > 0.0f)
        {
            v.normal.x /= len;
            v.normal.y /= len;
            v.normal.z /= len;
        }
    }
}

void parse(std::string filename, std::vector<Vertex>& out_vertices, std::vector<uint32_t>& out_indices)
{
    if ((filename.size() < 4) || (filename.substr(filename.size() - 4, 4) != std::string(".obj")))
        throw ParsingException("bad extension");
    std::ifstream myfile(filename);
    if (!myfile.is_open())
        throw ParsingException("cannot open file");
    std::string line;
    std::vector<Vec3> vertices;
    std::vector<Vec2> texCoords;
    std::vector<Vec3> normals;
    std::map<Vertex, uint32_t> uniqueVertices;
    while (std::getline(myfile, line))
    {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;
        if (keyword.empty() || keyword[0] == '#')
            continue;
        else if (keyword == "v")
        {
            float x, y, z;
            if (!(iss >> x >> y >> z))
                throw ParsingException("incomplete line (keyword v)");
            vertices.push_back(Vec3{x, y, z});
        }
        else if (keyword == "vt")
        {
            float u, v;
            if (!(iss >> u >> v))
                throw ParsingException("incomplete line (keyword vt)");
            texCoords.push_back(Vec2{u, 1.0f - v}); // Inversion du V pour Vulkan
        }
        else if (keyword == "vn")
        {
            float nx, ny, nz;
            if (!(iss >> nx >> ny >> nz))
                throw ParsingException("incomplete line (keyword vn)");
            normals.push_back(Vec3{nx, ny, nz});
        }
        else if (keyword == "vp")
        {
            continue;
        }
        else if (keyword == "o")
        {
            continue;
        }
        else if (keyword == "g")
        {
            continue;
        }
        else if (keyword == "s")
        {
            continue;
        }
        else if (keyword == "mtllib")
        {
            continue;
        }
        else if (keyword == "usemtl")
        {
            continue;
        }
        else if (keyword == "f")
        {
            std::string token;
            std::vector<std::string> faceTokens;
            while (iss >> token)
                faceTokens.push_back(token);
            for (size_t i = 1; i + 1 < faceTokens.size(); ++i)
            {
                std::string cornerTokens[3] = { faceTokens[0], faceTokens[i], faceTokens[i + 1] };
                for (int c = 0; c < 3; ++c)
                {
                    int vIdx = 0, vtIdx = 0, vnIdx = 0;
                    std::sscanf(cornerTokens[c].c_str(), "%d/%d/%d", &vIdx, &vtIdx, &vnIdx);
                    if (vtIdx == 0 && cornerTokens[c].find("//") != std::string::npos)
                        std::sscanf(cornerTokens[c].c_str(), "%d//%d", &vIdx, &vnIdx);
                    Vertex vertex{};
                    if (vIdx > 0 && static_cast<size_t>(vIdx) <= vertices.size())
                        vertex.position = vertices[vIdx - 1];
                    if (vtIdx > 0 && static_cast<size_t>(vtIdx) <= texCoords.size())
                        vertex.uv = texCoords[vtIdx - 1];
                    if (vnIdx > 0 && static_cast<size_t>(vnIdx) <= normals.size())
                        vertex.normal = normals[vnIdx - 1];
                    if (uniqueVertices.count(vertex) == 0) {
                        uint32_t newIndex = static_cast<uint32_t>(out_vertices.size());
                        uniqueVertices[vertex] = newIndex;
                        out_vertices.push_back(vertex);
                        out_indices.push_back(newIndex);
                    } else {
                        out_indices.push_back(uniqueVertices[vertex]);
                    }
                }
            }
        }
        else
        {
            throw ParsingException("caca prout token pas expected");
        }
    }
    computeNormalsIfMissing(out_vertices, out_indices, !normals.empty());
}