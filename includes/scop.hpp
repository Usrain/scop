#ifndef SCOP_HPP
# define SCOP_HPP
# include <iostream>
# include <vector>
# include <sstream>
# include <cstdint>
# include "scopexeption.hpp"
# include "m4.hpp"
# include <tuple>
struct Vec3 {
    float x, y, z;
    Vec3 operator+(const Vec3& other) const
    {
        return (Vec3{x + other.x, y + other.y, z + other.z});
    }
    Vec3 operator-(const Vec3& other) const
    {
        return (Vec3{x - other.x, y - other.y, z - other.z});
    }
};
struct Vec2 { float u, v; };

struct Vertex {
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
    bool operator<(const Vertex& other) const {
        return std::tie(position.x, position.y, position.z, uv.u, uv.v, normal.x, normal.y, normal.z) <std::tie(other.position.x, other.position.y, other.position.z, other.uv.u, other.uv.v, other.normal.x, other.normal.y, other.normal.z);
    };
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};
void parse(std::string filename, std::vector<Vertex>& out_vertices, std::vector<uint32_t>& out_indices);
#endif