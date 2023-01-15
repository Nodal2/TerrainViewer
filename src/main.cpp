#include <iostream>

#include "display.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "controller.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "heightfield.hpp"
#include "terrain_gui.hpp"

void update_controller_state(ControlState &controller_state, const MouseState &mouse_state, const KeyboardState &keyboard_state);
Scene create_scene(TerrainGuiState &gui_state);

int main()
{
    /* === create window === */
    Display display(1600, 900);
    display.init();

    /* === create gui === */
    TerrainGuiState gui_state;
    create_gui(display.window());

    /* === init scene === */
    Scene scene = create_scene(gui_state);

    /* === create controller === */
    OrbiterController controller({0.0f, 0.0f, 0.0f}, 0.01f, 0.005f, {0.5f, 0.5f, 0.0f});
    ControlState controller_state;

    /* === main loop === */
    bool window_closed = false;
    while(!window_closed)
    {
        /* === clear screen === */
        display.clear_screen(White());

        /* === new gui frame === */
        new_gui_frame();

        /* === process events === */
        display.poll_events();

        if(display.keyboard_state().escape)
            window_closed = true;

        update_controller_state(controller_state, display.mouse_state(), display.keyboard_state());

        if(!display.keyboard_state().alt)
        {
            display.hide_cursor();
            controller.update(controller_state);
            scene.camera().update(controller.position, controller.direction, controller.up);
        }
        else
            display.show_cursor();
        
        if(gui_state.update)
        {
            scene.destroy();
            scene = create_scene(gui_state);
            gui_state.update = false;
        }
        
        /* === render scene === */
        scene.camera().aspect_ratio = display.aspect_ratio();
        scene.camera().update(controller.position, controller.direction, controller.up);
        scene.draw();

        /* === render gui === */
        render_gui(gui_state);

        /* === terminate frame === */
        display.end_frame();
    }

    /* === cleanup === */
    scene.destroy();
    display.destroy();

    return 0;
}

Scene create_scene(TerrainGuiState &gui_state)
{
    /* === create shaders === */
    Shader shader = make_shader("../data/shader/basic_vertex.vs", "../data/shader/basic_fragment.fs");

    /* === create camera === */
    Camera camera;
    
    /* === create instances === */
    //Image height_map("../data/image/test2.jpg");
    //HeightField field(height_map, 0.15f, {0.0f, 0.0f}, {1.0f, 1.0f});
    //field.blur(2);
    
    HeightField field({0.0f, 0.0f}, {1.0f, 1.0f}, 250, 250);  
    field.perlin_noise(10.0f, 10.0f, 0.1f);
    
    for(int i = 0; i < gui_state.nb_iterations; ++i)
    {
        field.thermal_erosion(gui_state.thermal_quantity);
        field.stream_power_erosion(gui_state.k, gui_state.n);
    }

    field.fill(gui_state.water_level);

    if(gui_state.x1 != gui_state.x2 || gui_state.y1 != gui_state.y2)
        field.road(gui_state.x1, gui_state.y1, gui_state.x2, gui_state.y2, gui_state.width,
            gui_state.slope_cost, gui_state.water_low_cost, gui_state.water_high_cost, gui_state.water_treshold);

    std::string texture_path = "../data/image/height.png";
    if(gui_state.texture)
    {
        texture_path = "../data/image/texture.png";
        field.export_texture(texture_path);
    }
    if(gui_state.height)
    {
        texture_path = "../data/image/height.png";
        field.export_data(texture_path);
    }
    if(gui_state.slope)
    {
        texture_path = "../data/image/slope.png";
        field.export_gradient(texture_path);
    }
    if(gui_state.laplacian)
    {
        texture_path = "../data/image/laplacian.png";
        field.export_laplacian(texture_path);
    }
    if(gui_state.wetness)
    {
        texture_path = "../data/image/wetness.png";
        field.export_wetness(texture_path);
    }
    if(gui_state.stream_areas)
    {
        texture_path = "../data/image/stream_areas.png";
        field.export_stream_areas(texture_path);
    }

    std::vector<Vector3<float>> positions;
    std::vector<Vector2<float>> texture_coords;
    std::vector<unsigned int> indices;
    field.polygonize(positions, texture_coords, indices);
    Texture texture = make_texture(texture_path);
    Matrix4<float> transform = Identity<float>();

    Model model = make_model(positions, texture_coords, indices, {transform}, shader, texture);
    return Scene({model}, camera);
}

// binds keys / mouse to controllers actions
void update_controller_state(ControlState &controller_state, const MouseState &mouse_state, const KeyboardState &keyboard_state)
{
    controller_state.forward = keyboard_state.w;
    controller_state.backward = keyboard_state.s;
    controller_state.right = keyboard_state.d;
    controller_state.left = keyboard_state.a;
    controller_state.up = keyboard_state.space;
    controller_state.down = keyboard_state.shift;
    controller_state.x_look = mouse_state.offset_x;
    controller_state.y_look = mouse_state.offset_y;
    controller_state.zoom = mouse_state.scroll;
}