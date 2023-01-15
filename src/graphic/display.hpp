#ifndef MESHTOOL_DISPLAY
#define MESHTOOL_DISPLAY

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "color.hpp"

struct MouseState
{
    float x, y, previous_x, previous_y, offset_x, offset_y;
    bool scroll_changed;
    float scroll;
};

struct KeyboardState
{
    bool w, a, s, d, alt, space, shift, escape;
};

void mouse_callback(GLFWwindow *window, double x, double y);
void scroll_callback(GLFWwindow *window, double x, double y);

class Display
{
    struct FPS
    {
        double fps;
        double previous_time;
        double current_time;
        double delta_time;
        unsigned int frame_count;
    };

public:
    Display(unsigned int window_width, unsigned int window_height, const std::string &title = "MeshTool");
    void init();
    void clear_screen(const Color &screen_color);
    void poll_events();
    void end_frame();
    void show_cursor();
    void hide_cursor();
    void destroy();
    GLFWwindow *window();
    unsigned int window_width() const;
    unsigned int window_height() const;
    double fps() const;
    float aspect_ratio() const;
    const MouseState &mouse_state() const;
    const KeyboardState &keyboard_state() const;

private:
    void update_fps();
    
private:
    unsigned int window_width_, window_height_;
    std::string title_;
    GLFWwindow* window_;
    FPS fps_;
    MouseState mouse_state_;
    KeyboardState keyboard_state_;
};

#endif