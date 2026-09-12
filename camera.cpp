#include "includes/camera.hpp"
#include "includes/scop.hpp"
#include <cmath>
#include "includes/scop.hpp"
Camera::Camera(Vec3 pos, Vec3 target, Vec3 Up, float fov, float aspect, float near, float far)
    : pos(pos),up(Up), fov(fov), aspect(aspect), near(near), far(far)
{
    Vec3 dir = (target-pos).normalize();
    pitch = asinf(dir.y);
    yaw = atan2f(dir.z, dir.x);
    updateVectors();
}

m4 Camera::getView() const
{
    return m4::lookAt(pos, pos + front, up);
}

m4 Camera::getProjection() const
{
    return m4::perspective(fov, aspect, near, far);
}

void Camera::move(const Vec3& delta)
{
    pos = pos + delta;
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
    yaw += deltaYaw;
    pitch += deltaPitch;
    const float maxPitch = 89.0f * (M_PI / 180.0f);
    if (pitch > maxPitch) pitch = maxPitch;
    if (pitch < -maxPitch) pitch = -maxPitch;

    updateVectors();
}
void Camera::setAspect(float newAspect)
{
    aspect = newAspect;
}
void Camera::updateVectors()
{
    Vec3 newFront;
    newFront.x = cosf(yaw) * cosf(pitch);
    newFront.y = sinf(pitch);
    newFront.z = sinf(yaw) * cosf(pitch);

    front = newFront.normalize();
    right = front.cross(worldUp).normalize();
    up = right.cross(front);
}
Vec3 &Camera::getfront()
{
    return (this->front);
}
Vec3 &Camera::getright()
{
    return (this->right);
}
Vec3 &Camera::getup()
{
    return (this->up);
}