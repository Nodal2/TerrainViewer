#ifndef MESHTOOL_CAMERA
#define MESHTOOL_CAMERA

#include "vector.hpp"
#include "matrix.hpp"

class Camera
{
public:
    Camera();
    Camera(float fov, float aspect_ratio, float z_near, float z_far, const Vector3<float> &position, const Vector3<float> &direction, const Vector3<float> &up);
    void update(const Vector3<float> &position, const Vector3<float> &direction, const Vector3<float> &up);
    Matrix4<float> view() const;
    Matrix4<float> projection() const;
    Vector3<float> position() const;

public:
    float fov, aspect_ratio, z_near, z_far;

private:
    Vector3<float> position_, direction_, up_;
    
};

#endif