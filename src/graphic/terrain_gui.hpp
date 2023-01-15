#ifndef MESHTOOL_TERRAIN_GUI
#define MESHTOOL_TERRAIN_GUI

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <math.h>
#include <iostream>

struct TerrainGuiState
{
    // erosion
    float k = 0.0f;
    float n = 1.0f;
    float thermal_quantity = 0.0f;
    int nb_iterations = 0;

    // road
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int width = 2;
    float slope_cost = 0.0f;
    float water_low_cost = 1.0f;
    float water_high_cost = 100.0f;
    float water_treshold = 0.01f;

    // water level
    float water_level = 0.05f;

    // texture
    bool texture       = false;
    bool height        = false;
    bool slope         = false;
    bool laplacian     = false;
    bool wetness       = false;
    bool stream_areas  = false;

    // update
    bool update = false;
};

inline ImGuiWindowFlags gui_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

void create_gui(GLFWwindow *window);
void render_gui(TerrainGuiState &gui_state);
void new_gui_frame();
void render_combo();

#endif