#ifndef MESHTOOL_HEIGHTFIELD
#define MESHTOOL_HEIGHTFIELD

#include <array>

#include "scalarfield.hpp"
#include "perlin_noise.hpp"
#include "dijkstra.hpp"
#include "image.hpp"
#include "color.hpp"

class HeightField : public ScalarField
{
struct Cell
{
    unsigned int i, j;
    float height;
    float slope;
};

struct Vicinity
{
    std::vector<Cell> neighbors;
};

public:
    HeightField(const Vector2<float> &p_min, const Vector2<float> &p_max, unsigned int nx, unsigned int ny);
    HeightField(const Image &height_map, float scale_z, const Vector2<float> &p_min, const Vector2<float> &p_max);
    
    void polygonize(std::vector<Vector3<float>> &positions, std::vector<Vector2<float>> &textures_coords, std::vector<unsigned int> &indices) const;
    
    void perlin_noise(float fx, float fy, float height);
    void thermal_erosion(float quantity);
    void stream_power_erosion(float k, float n);
    void road(unsigned int i, unsigned int j, unsigned int gi, unsigned int gj, int width, float slope_cost, float water_low_cost, float water_high_cost, float water_treshold);
    void blur(unsigned int size);
    void fill(float height);
    
    void export_stream_areas(const std::string &path) const;
    void export_wetness(const std::string &path) const;
    void export_texture(const std::string &path) const;
    
    Vector3<float> point(unsigned int i, unsigned int j) const;
    Vector3<float> normal(unsigned int i, unsigned int j) const;

private:
    std::vector<float> stream_areas() const;
    std::vector<Cell> sorted_cells() const;
    float directional_slope(unsigned int i, unsigned int j, unsigned int ni, unsigned int nj) const;
    
    Vicinity vicinity_M14(const Cell &cell) const;
    Vicinity vicinity_M18(const Cell &cell) const;
    Vicinity vicinity_M2(const Cell &cell) const;
    Vicinity vicinity_M3(const Cell &cell) const;
    
    DijkstraAdjacencyList create_graph(float slope_cost, float water_low_cost, float water_high_cost, float water_treshold) const;
    std::vector<Cell> shortest_path(unsigned int i, unsigned int j, unsigned int gi, unsigned int gj, 
        float slope_cost, float water_low_cost, float water_high_cost, float water_treshold) const;

private:
    std::vector<float> water_;
};

#endif