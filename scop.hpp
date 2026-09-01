#ifndef SCOP_HPP
# define SCOP_HPP
# include <iostream>
# include <vector>
# include <sstream>
struct Vec3 { float x, y, z; };
struct Vec2 { float u, v; };

struct Vertex {
    Vec3 position;
    Vec2 uv;
    Vec3 normal;
    Vec3 color;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};
#endif