#include "scop.hpp"
struct Vec3;
class m4
{
    private :
        float m[16];
    public:
        m4();
        m4(float m00, float m01, float m02, float m03,float m10, float m11, float m12, float m13,float m20, float m21, float m22, float m23,float m30, float m31, float m32, float m33);
        m4(float val);
        float& operator[](int index);
        const float& operator[](int index) const;                              
        m4 operator*(const m4& b) const;
        static m4 identity();
        static m4 rotateX(float rangle);
        static m4 rotateY(float rangle); // angle in rad
        static m4 rotateZ(float rangle);
        static m4 translate(const Vec3& t);
        static m4 scale(const Vec3& s);
        static m4 zero();
        static m4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
        static m4 perspective(float fovyInRadians, float aspect, float zNear, float zFar);
};