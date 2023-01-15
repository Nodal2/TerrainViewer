#ifndef MESHTOOL_IMAGE
#define MESHTOOL_IMAGE

#include <vector>
#include <cassert>
#include <iostream>

#include "color.hpp"
#include "image_io.hpp"

class Image
{
public:
    Image(const std::string &path);
    Image(const std::vector<unsigned char> &pixels, unsigned int width, unsigned int height, unsigned int nb_channels);
    std::vector<unsigned char> pixels() const;
    unsigned int width() const;
    unsigned int height() const;
    unsigned int nb_channels() const;

private:
    unsigned int get_index(unsigned int x, unsigned int y) const;

private:
    std::vector<unsigned char> pixels_;
    unsigned int width_, height_, nb_channels_;  
};

#endif