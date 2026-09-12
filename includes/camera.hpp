#ifndef CAMERA_HPP
# define CAMERA_HPP
# include "vector.hpp"
# include "scop.hpp"
class Camera {
    public:
        Camera(Vec3 pos, Vec3 target, Vec3 up, float fov, float aspect, float near, float far);
        m4 getView() const; 
        m4 getProjection() const;
        void move(const Vec3& delta);
        void rotate(float yaw, float pitch);
        void setAspect(float aspect);
        void updateVectors(void);
        Vec3& getfront();
        Vec3& getright();
        Vec3& getup();
    private:
        Vec3 pos;
        Vec3 front, up, right;
        float yaw, pitch;
        float fov, aspect, near, far;
        const Vec3 worldUp = {0.0f, 1.0f, 0.0f};
};
#endif