#include "texture.hpp"

void Texture::init(const std::string &path, unsigned int unit)
{
    unit_ = unit;
    Image image(path);
    
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned int nb_channels;
    switch(image.nb_channels())
    {
        case(3) : nb_channels = GL_RGB; break;
        case(4) : nb_channels = GL_RGBA; break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, nb_channels, image.width(), image.height(), 0, nb_channels, GL_UNSIGNED_BYTE, &(image.pixels()[0]));
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::init(const Image &image, unsigned int unit)
{
    unit_ = unit;
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned int nb_channels;
    switch(image.nb_channels())
    {
        case(3) : nb_channels = GL_RGB; break;
        case(4) : nb_channels = GL_RGBA; break;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, nb_channels, image.width(), image.height(), 0, nb_channels, GL_UNSIGNED_BYTE, &(image.pixels()[0]));
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::use()
{
    glActiveTexture(GL_TEXTURE0 + unit_);
    glBindTexture(GL_TEXTURE_2D, texture_);
}

unsigned int Texture::texture_id() const
{
    return texture_;
}

unsigned int Texture::unit() const
{
    return unit_;
}

Texture make_texture(const std::string &path, unsigned int unit)
{
    Texture texture;
    texture.init(path, unit);
    return texture;
}