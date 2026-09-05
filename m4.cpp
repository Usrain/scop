#include "includes/scop.hpp"
#include <cmath>

m4::m4()
{
    m[0]  = 1.0f; m[1]  = 0.0f; m[2]  = 0.0f; m[3]  = 0.0f;
    m[4]  = 0.0f; m[5]  = 1.0f; m[6]  = 0.0f; m[7]  = 0.0f;
    m[8]  = 0.0f; m[9]  = 0.0f; m[10] = 1.0f; m[11] = 0.0f;
    m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f; 
}
m4::m4(float val) {
    for (int i = 0; i < 16; ++i)
        m[i] = val;
}
m4::m4(float m00, float m01, float m02, float m03,float m10, float m11, float m12, float m13,float m20, float m21, float m22, float m23,float m30, float m31, float m32, float m33) {
    m[0]  = m00; m[1]  = m01; m[2]  = m02; m[3]  = m03;
    m[4]  = m10; m[5]  = m11; m[6]  = m12; m[7]  = m13;
    m[8]  = m20; m[9]  = m21; m[10] = m22; m[11] = m23;
    m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
}
m4 m4::identity() {
    return (m4());
}
float& m4::operator[](int index)
{
    return (m[index]);
}
const float& m4::operator[](int index) const
{
    return (m[index]);
}
m4 m4::operator*(const m4& right) const {
    m4 result(0.0f);
    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k)
                sum += (*this)[k * 4 + row] * right[col * 4 + k];
            result[col * 4 + row] = sum;
        }
    }
    return result;
}

m4 m4::rotateY(float radians) {
    m4 result;
    float c = cosf(radians);
    float s = sinf(radians);
    result[0] = c;
    result[2] = -s;
    result[8] = s;
    result[10] = c;
    return result;
}

m4 m4::rotateX(float radians) {
    m4 result;
    float c = cosf(radians);
    float s = sinf(radians);
    result[5] = c;
    result[6] = s;
    result[9] = -s;
    result[10] = c;
    return result;
}

m4 m4::rotateZ(float radians) {
    m4 result;
    float c = cosf(radians);
    float s = sinf(radians);
    result[0] = c;
    result[1] = s;
    result[4] = -s;
    result[5] = c;
    return result;
}

m4 m4::translate(const Vec3& t)
{
    m4 result;
    result[12] = t.x;
    result[13] = t.y;
    result[14] = t.z;
    return result;
}

m4 m4::scale(const Vec3& s)
{
    m4 result;
    result[0] = s.x;
    result[5] = s.y;
    result[10] = s.z;
    return result;
}
m4 m4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) 
{
    Vec3 f = (center - eye).normalize();
    Vec3 r = f.cross(up).normalize();
    Vec3 u = r.cross(f);

    m4 result = m4::identity();

    result[0] = r.x;  result[4] = r.y;  result[8]  = r.z;
    
    result[1] = -u.x; result[5] = -u.y; result[9]  = -u.z; 
    
    result[2] = -f.x; result[6] = -f.y; result[10] = -f.z;

    result[12] = -r.dot(eye);
    result[13] =  u.dot(eye);
    result[14] =  f.dot(eye);
    result[15] =  1.0f;

    return result;
}

m4 m4::zero()
{
    return (m4 (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0));
}

m4 m4::perspective(float fovyInRadians, float aspect, float zNear, float zFar) 
{
    float tanHalfFovy = tanf(fovyInRadians / 2.0f);

    m4 result = m4::zero();
    result[0] = 1.0f / (aspect * tanHalfFovy);
    result[5] = -1.0f / tanHalfFovy; 
    result[10] = zFar / (zNear - zFar);
    result[11] = -1.0f;
    result[14] = -(zFar * zNear) / (zFar - zNear);

    return result;
}