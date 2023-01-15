#include "scene.hpp"

Scene::Scene(const std::vector<Model> &models, const Camera &camera) : models_(models), camera_(camera)
{
    assert(!models_.empty() && "cannot create scene without model");
    sort_models();
    last_model_ = models_.at(0);
    last_model_.shader().use();
    last_model_.texture().use();
}

void Scene::destroy()
{
    for(Model &model : models_)
        model.destroy();
}

Camera &Scene::camera()
{
    return camera_;
}

void Scene::sort_models()
{
    std::sort(models_.begin(), models_.end(), 
        [](const Model &lhs, const Model &rhs)
        {
            if(lhs.shader().program_id() != rhs.shader().program_id())
                return lhs.shader().program_id() < rhs.shader().program_id();
            return lhs.texture().texture_id() < rhs.texture().texture_id();
        }
    );
}

void Scene::draw()
{
    for(Model &model : models_)
    {
        if(last_model_.shader().program_id() != model.shader().program_id())
        {
            model.shader().use();
            std::cout << "shader changed" << std::endl;
        }
            
        if(last_model_.texture().texture_id() != model.texture().texture_id())
        {
            model.texture().use();
            std::cout << "color texture changed" << std::endl;
        }

        last_model_ = model;
        last_model_.shader().set_uniform("u_view_matrix", camera_.view());
        last_model_.shader().set_uniform("u_projection_matrix", camera_.projection());
        last_model_.shader().set_uniform("u_color_texture", (int)(model.texture().unit()));
        glBindVertexArray(last_model_.vao());
        glDrawElementsInstanced(GL_TRIANGLES, last_model_.nb_indices(), GL_UNSIGNED_INT, 0, last_model_.nb_instances());
    }
}