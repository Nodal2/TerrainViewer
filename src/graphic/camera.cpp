#include "camera.hpp"

Camera::Camera(float fov, float aspect_ratio, float z_near, float z_far, const Vector3<float> &position, const Vector3<float> &direction, const Vector3<float> &up)
    : fov(fov), aspect_ratio(aspect_ratio), z_near(z_near), z_far(z_far), position_(position), direction_(direction), up_(up)
{}

Camera::Camera() : fov(M_PI / 4), aspect_ratio(1.0f), z_near(0.1f), z_far(10.0f), position_({0.0f, 0.0f, 0.0f}), direction_({0.0f, 0.0f, 1.0f}), up_(0.0f, 1.0f, 0.0f)
{}

void Camera::update(const Vector3<float> &position, const Vector3<float> &direction, const Vector3<float> &up)
{
    position_ = position;
    direction_ = direction;
    up_ = up;
}

Matrix4<float> Camera::view() const
{
    return View(direction_, up_, position_);
}

Matrix4<float> Camera::projection() const
{
    return Projection(fov, aspect_ratio, z_near, z_far);
}

Vector3<float> Camera::position() const
{
    return position_;
}