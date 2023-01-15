#include "terrain_gui.hpp"

void create_gui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    style.Colors[ImGuiCol_TitleBg] = ImColor(70, 70, 70);
    style.Colors[ImGuiCol_TitleBgActive] = ImColor(70, 70, 70);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void render_gui(TerrainGuiState &gui_state)
{
    ImGui::Begin("Erosion", nullptr, gui_flags);
    ImGui::SetWindowPos(ImVec2(10.0f, 10.0f));
    ImGui::SliderFloat("k", &gui_state.k, 0.0f, 0.0001f, "%.8f");
    ImGui::SliderFloat("n", &gui_state.n, 0.0f, 2.00f, "%.5f");
    ImGui::SliderFloat("thermal quantity", &gui_state.thermal_quantity, 0.0f, 0.01f, "%.6f");
    ImGui::SliderInt("iterations", &gui_state.nb_iterations, 0, 200);
    ImGui::End();

    ImGui::Begin("Road", nullptr, gui_flags);
    ImGui::SetWindowPos(ImVec2(10.0f, 150.0f));
    ImGui::SliderInt("x1", &gui_state.x1, 0, 1000);
    ImGui::SliderInt("y1", &gui_state.y1, 0, 1000);
    ImGui::SliderInt("x2", &gui_state.x2, 0, 1000);
    ImGui::SliderInt("y2", &gui_state.y2, 0, 1000);
    ImGui::SliderInt("width", &gui_state.width, 1, 10);
    ImGui::SliderFloat("slope cost", &gui_state.slope_cost, 0.0f, 10.0f, "%.6f");
    ImGui::SliderFloat("water low cost", &gui_state.water_low_cost, 0.0f, 100.0f, "%.6f");
    ImGui::SliderFloat("water high cost", &gui_state.water_high_cost, 0.0f, 500.0f, "%.6f");
    ImGui::SliderFloat("water treshold", &gui_state.water_treshold, 0.0f, 0.3f, "%.6f");
    ImGui::End();

    ImGui::Begin("Water", nullptr, gui_flags);
    ImGui::SetWindowPos(ImVec2(10.0f, 400.0f));
    ImGui::SliderFloat("water level", &gui_state.water_level, 0.0f, 0.5f, "%.8f");
    ImGui::End();

    ImGui::Begin("Texture", nullptr, gui_flags);
    ImGui::SetWindowPos(ImVec2(10.0f, 470.0f));
    const char* items[] = {"texture", "height", "slope", "laplacian", "wetness", "stream_areas"};
    static const char* current_item = NULL;
    ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

    ImGuiStyle& style = ImGui::GetStyle();
    float w = ImGui::CalcItemWidth();
    float spacing = style.ItemInnerSpacing.x;
    float button_sz = ImGui::GetFrameHeight();
    ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
    if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (current_item == items[n]);
            if(ImGui::Selectable(items[n], is_selected))
            {
                current_item = items[n];
                gui_state.texture       = (current_item == items[0]);
                gui_state.height        = (current_item == items[1]);
                gui_state.slope         = (current_item == items[2]);
                gui_state.laplacian     = (current_item == items[3]);
                gui_state.wetness       = (current_item == items[4]);
                gui_state.stream_areas  = (current_item == items[5]);
            }
            if(is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::End();

    ImGui::Begin("Update", nullptr, gui_flags);
    ImGui::SetWindowPos(ImVec2(10.0f, 540.0f));
    if(ImGui::Button("update"))
        gui_state.update = true;
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void new_gui_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}