#include "model.hpp"

void Model::init(const std::vector<Vector3<float>> &positions, const std::vector<Vector3<float>> &normals, const std::vector<Vector2<float>> &texture_coords, 
    const std::vector<unsigned int> &indices, const std::vector<Matrix4<float>> &transforms, const Shader &shader, const Texture &texture)
{
    assert(positions.size() == texture_coords.size() && "vertex data sizes don't match");
    nb_vertices_ = positions.size();
    nb_indices_ = indices.size();
    nb_instances_ = transforms.size();
    create_vao(positions, normals, texture_coords, indices, transforms);
    shader_ = shader;
    texture_ = texture;
}

void Model::destroy()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &ebo_);
    glDeleteBuffers(1, &ibo_);
    glDeleteBuffers(1, &vbo_);
}

unsigned int Model::vao() const
{
    return vao_;
}

unsigned int Model::nb_vertices() const
{
    return nb_vertices_;
}

unsigned int Model::nb_indices() const
{
    return nb_indices_;
}

unsigned int Model::nb_instances() const
{
    return nb_instances_;
}

Shader Model::shader() const
{
    return shader_;
}

Texture Model::texture() const
{
    return texture_;
}

void Model::create_vao(const std::vector<Vector3<float>> &positions, const std::vector<Vector3<float>> &normals, const std::vector<Vector2<float>> &texture_coords, 
    const std::vector<unsigned int> &indices, const std::vector<Matrix4<float>> &transforms)
{
    std::vector<Vector3<float>> transform_columns;
    transform_columns.reserve(nb_instances_ * 4);
    for(const Matrix4<float> &transform : transforms)
    {
        for(unsigned int i = 0; i < 4; ++i)
            transform_columns.push_back(transform[i]);
    }

    glGenVertexArrays(1, &vao_);
    glCreateBuffers(1, &vbo_);
    glCreateBuffers(1, &ibo_);
    glCreateBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    // store vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, nb_vertices_ * (sizeof(Vector3<float>) * 2 + sizeof(Vector2<float>)), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, nb_vertices_ * sizeof(Vector3<float>), &positions[0]);
    glBufferSubData(GL_ARRAY_BUFFER, nb_vertices_ * sizeof(Vector3<float>), nb_vertices_ * sizeof(Vector3<float>), &normals[0]);
    glBufferSubData(GL_ARRAY_BUFFER, nb_vertices_ * sizeof(Vector3<float>) * 2, nb_vertices_ * sizeof(Vector2<float>), &texture_coords[0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3<float>), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3<float>), (void*)(nb_vertices_ * sizeof(Vector3<float>)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2<float>), (void*)(nb_vertices_ * sizeof(Vector3<float>) * 2));

    // store per instances vertex data
    glBindBuffer(GL_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ARRAY_BUFFER, transform_columns.size() * sizeof(Vector3<float>), &transform_columns[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3<float>) * 4, (void *)0);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3<float>) * 4, (void *)(sizeof(Vector3<float>)));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3<float>) * 4, (void *)(sizeof(Vector3<float>) * 2));
    glVertexAttribDivisor(5, 1);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3<float>) * 4, (void *)(sizeof(Vector3<float>) * 3));
    glVertexAttribDivisor(6, 1);

    // store indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nb_indices_ * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Model make_model(const std::vector<Vector3<float>> &positions, const std::vector<Vector2<float>> &texture_coords, 
        const std::vector<unsigned int> &indices, const std::vector<Matrix4<float>> &transforms, const Shader &shader, const Texture &texture)
{
    Model model;
    model.init(positions, normals(positions, indices), texture_coords, indices, transforms, shader, texture);
    return model;
}

std::vector<Vector3<float>> normals(const std::vector<Vector3<float>> &positions, const std::vector<unsigned int> &indices)
{
    std::vector<Vector3<float>> ns(positions.size(), Vector3<float>(0.0f, 0.0f, 0.0f));
    for(unsigned int i = 0; i < indices.size(); i += 3)
    {
        Vector3<float> ab = positions.at(indices.at(i + 1)) - positions.at(indices.at(i));
        Vector3<float> ac = positions.at(indices.at(i + 2)) - positions.at(indices.at(i));
        Vector3<float> normal = cross(ab, ac);
        ns.at(indices.at(i)) = ns.at(indices.at(i)) + normal;
        ns.at(indices.at(i + 1)) = ns.at(indices.at(i + 1)) + normal;
        ns.at(indices.at(i + 2)) = ns.at(indices.at(i + 2)) + normal;
    }
    for(unsigned int i = 0; i < ns.size(); ++i)
        ns.at(i) = normalize(ns.at(i));
    return ns;
}