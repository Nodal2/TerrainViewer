#ifndef MESHVIEWER_SHADER
#define MESHVIEWER_SHADER

#include <iostream>
#include <glad/glad.h>

#include "util.hpp"
#include "vector.hpp"
#include "matrix.hpp"

class Shader
{
public:
    void init(const std::string &vertex_path, const std::string &fragment_path);
    void use();
    void destroy();
    unsigned int program_id() const;
    void set_uniform(const std::string &name, int value);
    void set_uniform(const std::string &name, float value);
    void set_uniform(const std::string &name, bool value);
    void set_uniform(const std::string &name, const Vector3<float> &vector);
    void set_uniform(const std::string &name, const Matrix4<float> &matrix);

private:
    unsigned int compile_shader(const std::string &shader_source, GLuint type);
    unsigned int create_program(int vertex_shader, int fragment_shader);

private:
    unsigned int program_;
};

Shader make_shader(const std::string &vertex_path, const std::string &fragment_path);

#endif