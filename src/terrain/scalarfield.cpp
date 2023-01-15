#include "scalarfield.hpp"

ScalarField::ScalarField(const Vector2<float> &p_min, const Vector2<float> &p_max, unsigned int nx, unsigned int ny)
    : p_min_(p_min), p_max_(p_max), nx_(nx), ny_(ny)
{
    data_ = std::vector<float>(nx_ * ny_, 0);
    scale_x_ = (p_max_.x - p_min_.x) / (nx_ - 1);
    scale_y_ = (p_max_.y - p_min_.y) / (ny_ - 1);
}

float ScalarField::value(unsigned int i, unsigned int j) const
{
    return data_.at(index(i, j));
}

Vector2<float> ScalarField::gradient(unsigned int i, unsigned int j) const
{
    int min_x = i > 0 ? -1 : 0;
    int max_x = i < nx_ - 1 ? 1 : 0;
    int min_y = j > 0 ? -1 : 0;
    int max_y = j < ny_ - 1 ? 1 : 0;

    float grad_x = (value(i + min_x, j) - value(i + max_x, j)) / (2 * scale_x_);
    float grad_y = (value(i, j + min_y) - value(i, j + max_y)) / (2 * scale_y_);

    return Vector2<float>(grad_x, grad_y);
}

float ScalarField::slope(unsigned int i, unsigned int j) const
{
    return length(gradient(i, j));
}

float ScalarField::laplacian(unsigned int i, unsigned int j) const
{
    int min_x = i > 0 ? -1 : 0;
    int max_x = i < nx_ - 1 ? 1 : 0;
    int min_y = j > 0 ? -1 : 0;
    int max_y = j < ny_ - 1 ? 1 : 0;

    float num = value(i + min_x, j) + value(i + max_x, j) + value(i, j + min_y) + value(i, j + max_y) - (4 * value(i, j));

    return num / (scale_x_ * scale_y_);
}

void ScalarField::export_data(const std::string &path) const
{
    image_io::write_gray(path, data_, nx_, ny_);
}

void ScalarField::export_gradient(const std::string &path) const
{
    std::vector<float> magnitudes;
    magnitudes.reserve(nx_ * ny_);
    for(unsigned int j = 0; j < ny_; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
            magnitudes.push_back(slope(i, j));
    }
    image_io::write_gray(path, magnitudes, nx_, ny_);
}

void ScalarField::export_laplacian(const std::string &path) const
{
    std::vector<float> laplacians;
    laplacians.reserve(nx_ * ny_);
    for(unsigned int j = 0; j < ny_ ; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
            laplacians.push_back(laplacian(i, j));
    }
    image_io::write_gray(path, laplacians, nx_, ny_);
}

unsigned int ScalarField::index(unsigned int i, unsigned int j) const
{
    assert(i * j < data_.size() && "indices out of bounds");
    return (j * nx_) + i;
}

std::pair<unsigned int, unsigned int> ScalarField::coords(unsigned int index) const
{
    assert(index < nx_ * ny_ && "index out of bounds");
    unsigned int i = index % nx_;
    unsigned int j = (index - i) / ny_;
    return std::make_pair(i, j);
}

std::vector<float> ScalarField::data() const
{
    return data_;
}

Vector2<float> ScalarField::p_min() const
{
    return p_min_;
}

Vector2<float> ScalarField::p_max() const
{
    return p_max_;
}

unsigned int ScalarField::nx() const
{
    return nx_;
}

unsigned int ScalarField::ny() const
{
    return ny_;
}

float ScalarField::scale_x() const
{
    return scale_x_;
}

float ScalarField::scale_y() const
{
    return scale_y_;
}
