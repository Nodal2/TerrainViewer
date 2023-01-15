#include "shader.hpp"

void Shader::init(const std::string &vertex_path, const std::string &fragment_path)
{
    std::string vertex_source, fragment_source;
    bool vertex_read_status = util::read_file(vertex_path, vertex_source);
    bool fragment_read_status = util::read_file(fragment_path, fragment_source);

    if(!vertex_read_status || !fragment_read_status)
    {
        fprintf(stderr, "[FILE] - could not read vertex shader or fragment shader (%s, %s)\n", vertex_path.c_str(), fragment_path.c_str());
        exit(EXIT_FAILURE);
    }
    else
    {
        unsigned int vertex_shader = compile_shader(vertex_source, GL_VERTEX_SHADER);
        unsigned int fragment_shader = compile_shader(fragment_source, GL_FRAGMENT_SHADER);
        program_ = create_program(vertex_shader, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
}

void Shader::use()
{
    glUseProgram(program_);
}

void Shader::destroy()
{
    glDeleteProgram(program_);
}

unsigned int Shader::program_id() const
{
    return program_;
}

void Shader::set_uniform(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::set_uniform(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(program_, name.c_str()), value);
}

void Shader::set_uniform(const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()), (int)value);
}

void Shader::set_uniform(const std::string &name, const Matrix4<float> &matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_TRUE, &matrix.m[0][0]);
}

void Shader::set_uniform(const std::string &name, const Vector3<float> &vector)
{
    glUniform3f(glGetUniformLocation(program_, name.c_str()), vector.x, vector.y, vector.z);
}

unsigned int Shader::compile_shader(const std::string &shader_source, GLuint type)
{
    const char* source_c = shader_source.c_str();

    unsigned int shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source_c, NULL);
    glCompileShader(shader);
    
    int success;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log);
        fprintf(stderr, "[OPENGL error] - %s\n", log);
        exit(EXIT_FAILURE);
    }
    return shader;
}

unsigned int Shader::create_program(int vertex_shader, int fragment_shader)
{
    unsigned int program;
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        fprintf(stderr, "[OPENGL error] - %s\n", log);
        exit(EXIT_FAILURE);
    }
    return program;
}

Shader make_shader(const std::string &vertex_path, const std::string &fragment_path)
{
    Shader shader;
    shader.init(vertex_path, fragment_path);
    return shader;
}