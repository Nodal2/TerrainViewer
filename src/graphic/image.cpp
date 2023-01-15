#include "image.hpp"

Image::Image(const std::string &path)
{
    pixels_ = image_io::load(path, &width_, &height_, &nb_channels_, true);
}

Image::Image(const std::vector<unsigned char> &pixels, unsigned int width, unsigned int height, unsigned int nb_channels)
    : pixels_(pixels), width_(width), height_(height), nb_channels_(nb_channels)
{
    assert(pixels.size() == width * height * nb_channels && "pixel array size does not match the provided dimensions");
}

std::vector<unsigned char> Image::pixels() const
{
    return pixels_;
}

unsigned int Image::width() const
{
    return width_;
}

unsigned int Image::height() const
{
    return height_;
}

unsigned int Image::nb_channels() const
{
    return nb_channels_;
}

unsigned int Image::get_index(unsigned int x, unsigned int y) const
{
    assert(x < width_ && y < height_ && "indices out of bounds");
    return y * width_ + x;
}