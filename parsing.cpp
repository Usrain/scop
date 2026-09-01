#include "scop.hpp"
#include "fstream"
#include <sstream>
void parse(std::string filename)
{
    if ((filename.size() < 4) || (filename.substr(filename.size() - 4, 4) != std::string(".obj")))
        return ; // bad name
    std::ifstream myfile(filename);
    if (!myfile.is_open())
        return ; //missing file
    std::string line;
    std::vector<Vec3> vertices;
    std::vector<Vec2> texCoords;
    std::vector<Vec3> normals;
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
                return ; // ligne pas complete ou lisible
                std::string trailing;
            if (iss >> trailing)
                return; // trop de valeurs sur la ligne "v"
            vertices.push_back(Vec3{x, y, z});
        }
        else if (keyword == "vt")
        {
            float u, v;
            if (iss >> u >> v)
                texCoords.push_back(Vec2{u, 1.0f - v}); // Inversion du V pour Vulkan
        }
        else if (keyword == "vn")
        {
            continue;
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
        }
        else
        {
            return ; // caca prout pas expected
        }
    }
}