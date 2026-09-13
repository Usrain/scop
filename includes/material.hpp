#ifndef MATERIAL_HPP
# define MATERIAL_HPP
#include "scop.hpp"
#include "vector.hpp"
class material
{
public:
    std::string name;
    Vec3 Ka, Kd, Ks, Ke, Tf;
    float Ns, Ni, d, Tr;
    int illum;
    std::string map_Ka;
    std::string map_Kd;
    std::string map_Ks;
    std::string map_Ns;
    std::string map_d;
    std::string map_bump;
    material();
    ~material();
};
#endif