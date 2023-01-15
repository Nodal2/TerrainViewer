#ifndef MESHTOOL_MODEL
#define MESHTOOL_MODEL

#include <vector>
#include <cassert>

#include <glad/glad.h>

#include "vector.hpp"
#include "matrix.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Model
{
public:
    void init(const std::vector<Vector3<float>> &positions, const std::vector<Vector3<float>> &normals, const std::vector<Vector2<float>> &texture_coords, 
        const std::vector<unsigned int> &indices, const std::vector<Matrix4<float>> &transforms, const Shader &shader, const Texture &texture);
    void destroy();
    unsigned int vao() const;
    unsigned int nb_vertices() const;
    unsigned int nb_indices() const;
    unsigned int nb_instances() const;
    Shader shader() const;
    Texture texture() const;

private:
    void create_vao(const std::vector<Vector3<float>> &positions, const std::vector<Vector3<float>> &normals, const std::vector<Vector2<float>> &texture_coords, 
        const std::vector<unsigned int> &indices, const std::vector<Matrix4<float>> &transforms);

private:
    unsigned int vao_, vbo_, ibo_, ebo_;
    unsigned int nb_vertices_, nb_indices_, nb_instances_;
    Shader shader_;
    Texture texture_;
};

Model make_model(const std::vector<Vector3<float>> &positions, const std::vector<Vector2<float>> &texture_coords, 
        const std::vector<unsigned int> &indices, const std::vector<Matrix4<float>> &transforms, const Shader &shader, const Texture &texture);

std::vector<Vector3<float>> normals(const std::vector<Vector3<float>> &positions, const std::vector<unsigned int> &indices);

#endif