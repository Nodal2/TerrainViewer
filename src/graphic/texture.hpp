#ifndef MESHTOOL_TEXTURE
#define MESHTOOL_TEXTURE

#include <iostream>

#include <glad/glad.h>

#include "image.hpp"

class Texture
{
public:
    void init(const std::string &path, unsigned int unit = 0);
    void init(const Image &image, unsigned int unit = 0);
    void use();
    unsigned int texture_id() const;
    unsigned int unit() const;

private:
    unsigned int texture_;
    unsigned int unit_;
};

Texture make_texture(const std::string &path, unsigned int unit = 0);

#endif