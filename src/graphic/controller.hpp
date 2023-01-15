#ifndef MESHTOOL_CONTROLLER
#define MESHTOOL_CONTROLLER

#include "vector.hpp"
#include "matrix.hpp"

struct ControlState
{
    float x_look;
    float y_look;
    float zoom;
    int forward;
    int backward;
    int right;
    int left;
    int up;
    int down;
};

class Controller
{
public:
    Controller(const Vector3<float> &position, float speed, float sensitivity);
    virtual ~Controller() = default;
    virtual void update(ControlState &control_state) = 0;

public:
    Vector3<float> position;
    Vector3<float> direction;
    Vector3<float> up;
    float yaw_angle;
    float pitch_angle;
    float speed;
    float sensitivity;
};

class FreeController : public Controller
{
public:
    FreeController(const Vector3<float> &position, float speed, float sensitivity);
    virtual ~FreeController() = default;
    virtual void update(ControlState &control_state) override;
};

class OrbiterController : public Controller
{   
public:
    OrbiterController(const Vector3<float> &position, float zoom_speed, float sensitivity, const Vector3<float> &target_position);
    virtual ~OrbiterController() = default;
    virtual void update(ControlState &control_state) override;

public:
    Vector3<float> target_position;

private:
    float distance_;
};

#endif