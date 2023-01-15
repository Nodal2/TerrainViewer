#include "display.hpp"

Display::Display(unsigned int window_width, unsigned int window_height, const std::string &title)
    : window_width_(window_width), window_height_(window_height), title_(title), window_(nullptr)
{
    fps_.current_time = glfwGetTime();
    mouse_state_.previous_x = window_width_ / 2.0f;
    mouse_state_.previous_y = window_height_ / 2.0f;
}

void Display::init()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(window_width_, window_height_, title_.c_str(), NULL, NULL);
    
    if (!window_)
    {
        glfwTerminate();
        fprintf(stderr, "[GLFW] - could not create window");
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window_);
    glfwFocusWindow(window_);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window_, (void *)&mouse_state_);
    glfwSetCursorPosCallback(window_, mouse_callback);
    glfwSetScrollCallback(window_, scroll_callback);
    glfwSwapInterval(1);

    if(!gladLoadGL())
    {
        fprintf(stderr, "[GLAD] - GLAD initialization failed");
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
}

void Display::clear_screen(const Color &screen_color)
{
    glClearColor(screen_color.red, screen_color.green, screen_color.blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update_fps();
}

void Display::poll_events()
{
    glfwPollEvents();

    keyboard_state_.w = glfwGetKey(window_, GLFW_KEY_W);
    keyboard_state_.a = glfwGetKey(window_, GLFW_KEY_A);
    keyboard_state_.s = glfwGetKey(window_, GLFW_KEY_S);
    keyboard_state_.d = glfwGetKey(window_, GLFW_KEY_D);
    keyboard_state_.alt = glfwGetKey(window_, GLFW_KEY_LEFT_ALT);
    keyboard_state_.space = glfwGetKey(window_, GLFW_KEY_SPACE);
    keyboard_state_.shift = glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT);
    keyboard_state_.escape = glfwGetKey(window_, GLFW_KEY_ESCAPE);

    if(mouse_state_.x == mouse_state_.previous_x)
        mouse_state_.offset_x = 0;
    if(mouse_state_.y == mouse_state_.previous_y)
        mouse_state_.offset_y = 0;
    mouse_state_.x = mouse_state_.previous_x;
    mouse_state_.y = mouse_state_.previous_y;

    if(mouse_state_.scroll_changed)
        mouse_state_.scroll_changed = false;
    else
        mouse_state_.scroll = 0.0f;
}

void Display::end_frame()
{
    glfwSwapBuffers(window_);
}

void Display::show_cursor()
{
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Display::hide_cursor()
{
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Display::destroy()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Display::update_fps()
{
    fps_.current_time = glfwGetTime();
    fps_.delta_time = fps_.current_time - fps_.previous_time;
    ++fps_.frame_count;
    if(fps_.delta_time >= 1.0 / 30.0)
        fps_.fps = (1.0 / fps_.delta_time) * fps_.frame_count;
}

GLFWwindow *Display::window()
{
    return window_;
}

unsigned int Display::window_width() const
{
    return window_width_;
}

unsigned int Display::window_height() const
{
    return window_height_;
}

double Display::fps() const
{
    return fps_.fps;
}

float Display::aspect_ratio() const
{
    return window_width_ / (float)window_height_;
}

const MouseState &Display::mouse_state() const
{
    return mouse_state_;
}

const KeyboardState &Display::keyboard_state() const
{
    return keyboard_state_;
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
    MouseState *mouse_state = (MouseState *)glfwGetWindowUserPointer(window);
    mouse_state->offset_x = mouse_state->previous_x - x;
    mouse_state->offset_y = mouse_state->previous_y - y;
    mouse_state->previous_x = x;
    mouse_state->previous_y = y;
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
    MouseState *mouse_state = (MouseState *)glfwGetWindowUserPointer(window);
    mouse_state->scroll = -y;
    mouse_state->scroll_changed = true;
}