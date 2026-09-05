#include "scop.hpp"
#include "fstream"
#include <cstdint>
#include <sstream>
#include <map>
#include <tuple>

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
    while (std::getline(myfile, line))
    {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;
        std::map<Vertex, uint32_t> uniqueVertices;
        if (keyword.empty() || keyword[0] == '#')
            continue;
        else if (keyword == "v")
        {
            float x, y, z;
            if (!(iss >> x >> y >> z))
                throw ParsingException("incomplete line (keyword v)");
            std::string trailing;
            //if (iss >> trailing)
            //    return; // trop de valeurs sur la ligne "v"
            vertices.push_back(Vec3{x, y, z});
        }
        else if (keyword == "vt")
        {
            float u, v;
            if (!(iss >> u >> v))
                throw ParsingException("incomplete line (keyword vt)");
            std::string trailing;
            //if (iss >> trailing)
            //    return; // trop de valeurs sur la ligne "v"
            texCoords.push_back(Vec2{u, 1.0f - v}); // Inversion du V pour Vulkan
        }
        else if (keyword == "vn")
        {
            float nx, ny, nz;
            if (!(iss >> nx >> ny >> nz))
                throw ParsingException("incomplete line (keyword vn)");
            std::string trailing;
            //if (iss >> trailing)
            //    return; // trop de valeurs sur la ligne "v"
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
                    //la suite est de l'opti :
                    if (uniqueVertices.count(vertex) == 0) {
                        // nouveau sommet
                        uint32_t newIndex = static_cast<uint32_t>(out_vertices.size());
                        uniqueVertices[vertex] = newIndex;
                        out_vertices.push_back(vertex);
                        out_indices.push_back(newIndex);
                    } else {
                        // ancien sommet
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
}