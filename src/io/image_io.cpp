#include "image_io.hpp"

std::vector<unsigned char> image_io::load(const std::string &path, unsigned int *width, unsigned int *height, unsigned int *nb_channels, bool flip)
{
    std::vector<unsigned char> image;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(path.c_str(), (int *)width, (int *)height, (int *)nb_channels, 0);
    if(data == nullptr)
    {
        fprintf(stderr, "[IMAGE LOADER] - could not load image %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }
    printf("[IMAGE LOADER] - image loaded %s %d %d %d\n", path.c_str(), *width, *height, *nb_channels);
    image.assign(data, data + (*width) * (*height) * (*nb_channels));
    stbi_image_free(data);
    return image;
}

void image_io::write_gray(const std::string &path, const std::vector<float> &data, unsigned int width, unsigned int height)
{
    assert(data.size() == width * height && "data size does not match dimensions");
    std::vector<unsigned char> normalized_data;
    normalized_data.reserve(width * height * 4);
    float max = *std::max_element(data.begin(), data.end());
    float min = *std::min_element(data.begin(), data.end());

    for(unsigned int i = 0; i < width * height; ++i)
    {
        unsigned char value = 255 * ((data.at(i) - min) / (max - min));
        for(unsigned int j = 0; j < 3; ++j)
            normalized_data.push_back(value);
        normalized_data.push_back(255);
    }

    stbi_flip_vertically_on_write(true);
    stbi_write_png(path.c_str(), width, height, 4, &normalized_data[0], width * sizeof(unsigned char) * 4);
}

void image_io::write_color(const std::string &path, const std::vector<Color> &data, unsigned int width, unsigned int height)
{
    assert(data.size() == width * height && "data size does not match dimensions");
    std::vector<unsigned char> normalized_data;
    normalized_data.reserve(width * height * 4);

    for(unsigned int i = 0; i < width * height; ++i)
    {
        for(size_t j = 0; j < 4; ++j)
            normalized_data.push_back(data.at(i)[j] * 255);
    }

    stbi_flip_vertically_on_write(true);
    stbi_write_png(path.c_str(), width, height, 4, &normalized_data[0], width * sizeof(unsigned char) * 4);
}