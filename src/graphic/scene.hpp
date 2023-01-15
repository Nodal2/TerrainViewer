#ifndef MESHTOOL_SCENE
#define MESHTOOL_SCENE

#include <vector>
#include <algorithm>
#include <cassert>

#include <glad/glad.h>

#include "model.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Scene
{
public:
    Scene(const std::vector<Model> &models, const Camera &camera);
    void destroy();
    void draw();
    Camera &camera();

private:
    void sort_models();

private:
    std::vector<Model> models_;
    Model last_model_;
    Camera camera_;
};

#endif