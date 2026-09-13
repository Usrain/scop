#ifndef VECTOR_HPP
# define VECTOR_HPP
# include <cmath>
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
    Vec3 operator*(const float& other) const
    {
        return (Vec3{x * other , y * other, z * other});
    }
    Vec3 normalize()
    {
        float norm = (sqrtf(x*x + y*y + z*z));
        if (norm > 0.00001f) 
        {
            return Vec3{x / norm, y / norm, z / norm};
        }
        return Vec3{0.0f, 0.0f, 0.0f};
    }
    Vec3 cross(const Vec3& other) const
    {
        return Vec3{
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }
    float dot(const Vec3& other) const
    {
        return (x * other.x + y * other.y + z * other.z);
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
#endif