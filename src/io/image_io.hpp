#ifndef MESHTOOL_IMAGE_IO
#define MESHTOOL_IMAGE_IO

#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>

#include "stb_image.h"
#include "stb_image_write.h"
#include "color.hpp"

namespace image_io
{
    std::vector<unsigned char> load(const std::string &path, unsigned int *width, unsigned int *height, unsigned int *nb_channels, bool flip);
    void write_gray(const std::string &path, const std::vector<float> &data, unsigned int width, unsigned int height);
    void write_color(const std::string &path, const std::vector<Color> &data, unsigned int width, unsigned int height);
};

#endif