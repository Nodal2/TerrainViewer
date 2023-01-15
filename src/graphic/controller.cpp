#include "controller.hpp"

Controller::Controller(const Vector3<float> &position, float speed, float sensitivity) 
    : position(position), direction(Vector3<float>(0.0f, 0.0f, 1.0f)), 
        up(Vector3<float>(0.0f, 1.0f, 0.0f)), yaw_angle(M_PI / 2), 
        pitch_angle(0.0f), speed(speed), sensitivity(sensitivity)
{}

FreeController::FreeController(const Vector3<float> &position, float speed, float sensitivity) : Controller(position, speed, sensitivity)
{}

void FreeController::update(ControlState &control_state)
{
    Vector3<float> left;

    yaw_angle += control_state.x_look * sensitivity;
    pitch_angle -= control_state.y_look * sensitivity;
        
    direction = normalize(
        Vector3<float>(cos(yaw_angle) * cos(pitch_angle), 
        -sin(pitch_angle), 
        sin(yaw_angle) * cos(pitch_angle))
    );

    left = cross(direction, up);
    position = position + direction * speed * control_state.forward;        
    position = position - direction * speed * control_state.backward;
    position = position - left * speed * control_state.right;
    position = position + left * speed * control_state.left;
    position = position + up * speed * control_state.up;
    position = position - up * speed * control_state.down;
}

OrbiterController::OrbiterController(const Vector3<float> &position, float zoom_speed, float sensitivity, const Vector3<float> &target_position) 
    : Controller(position, zoom_speed, sensitivity), target_position(target_position), distance_(length(target_position - position))
{}

void OrbiterController::update(ControlState &control_state)
{
    position = target_position;
    distance_ += control_state.zoom * speed;
    yaw_angle += control_state.x_look * sensitivity;
    pitch_angle -= control_state.y_look * sensitivity;
        
    direction = normalize(
        Vector3<float>(cos(yaw_angle) * cos(pitch_angle), 
        -sin(pitch_angle), 
        sin(yaw_angle) * cos(pitch_angle))
    );
    position = position - (direction * distance_);
    
}