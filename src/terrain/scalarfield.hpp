#ifndef MESHTOOL_SCALARFIELD
#define MESHTOOL_SCALARFIELD

#include <vector>

#include "vector.hpp"
#include "image_io.hpp"

class ScalarField
{
public:
    ScalarField(const Vector2<float> &p_min, const Vector2<float> &p_max, unsigned int nx, unsigned int ny);

    float value(unsigned int i, unsigned int j) const;
    Vector2<float> gradient(unsigned int i, unsigned int j) const;
    float slope(unsigned int i, unsigned int j) const;
    float laplacian(unsigned int i, unsigned int j) const;

    void export_data(const std::string &path) const;
    void export_gradient(const std::string &path) const;
    void export_laplacian(const std::string &path) const;

    std::vector<float> data() const;
    Vector2<float> p_min() const;
    Vector2<float> p_max() const;
    unsigned int nx() const;
    unsigned int ny() const;
    float scale_x() const;
    float scale_y() const;

protected:
    unsigned int index(unsigned int i, unsigned int j) const;
    std::pair<unsigned int, unsigned int> coords(unsigned int index) const;

protected:
    std::vector<float> data_;
    Vector2<float> p_min_, p_max_;
    unsigned int nx_, ny_;
    float scale_x_, scale_y_;
};

#endif