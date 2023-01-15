#include "heightfield.hpp"

HeightField::HeightField(const Vector2<float> &p_min, const Vector2<float> &p_max, unsigned int nx, unsigned int ny)
    : ScalarField(p_min, p_max, nx, ny), water_(std::vector<float>(nx * ny, 0.0f))
{}

HeightField::HeightField(const Image &height_map, float scale_z, const Vector2<float> &p_min, const Vector2<float> &p_max)
    : ScalarField(p_min, p_max, height_map.width(), height_map.height()), water_(std::vector<float>(height_map.width() * height_map.height(), 0.0f))
{
    std::vector<unsigned char> pixels = height_map.pixels();
    std::vector<float> mapped_pixels;
    mapped_pixels.reserve(nx_ * ny_);
    for(unsigned int i = 0; i < nx_ * ny_ * height_map.nb_channels(); i += 3)
        mapped_pixels.push_back((pixels.at(i) / 255.0f) * scale_z);
    data_ = mapped_pixels;
}

void HeightField::polygonize(std::vector<Vector3<float>> &positions, std::vector<Vector2<float>> &texture_coords, std::vector<unsigned int> &indices) const
{
    positions.clear();
    texture_coords.clear();
    indices.clear();

    positions.reserve(nx_ * ny_);
    texture_coords.reserve(nx_ * ny_);
    indices.reserve(((nx_ - 1) * (ny_ - 1)) * 6);

    for(unsigned int j = 0; j < ny_; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
        {
            positions.push_back(point(i, j));
            texture_coords.push_back(Vector2<float>(i / (float)(nx_ - 1), j / (float)(ny_ - 1)));
            if(i < nx_ - 1 && j < ny_ - 1)
            {
                unsigned int i0 = index(i, j);
                unsigned int i1 = index(i + 1, j);
                unsigned int i2 = index(i, j + 1);
                unsigned int i3 = index(i + 1, j + 1);

                indices.push_back(i0);
                indices.push_back(i1);
                indices.push_back(i3);

                indices.push_back(i0);
                indices.push_back(i3);
                indices.push_back(i2);
            }
        }
    }
}

void HeightField::perlin_noise(float fx, float fy, float height)
{
    assert(height > 0.0f && "incorrect height value");
    for(unsigned int j = 0; j < ny_; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
            data_.at(index(i, j)) = (noise(i / (nx_ / fx), j / (ny_ / fy), 40)) / (1 / height);
    }
}

void HeightField::thermal_erosion(float quantity)
{
    std::vector<Cell> cells = sorted_cells();

    for(const Cell &cell : cells)
    {
        Vicinity vicinity = vicinity_M14(cell);
        float min_slope = 1000000.0f;
        unsigned int lowest_cell_index = 0;
        for(unsigned int i = 0; i < vicinity.neighbors.size(); ++i)
        {
            if(vicinity.neighbors.at(i).slope < min_slope)
            {
                min_slope = vicinity.neighbors.at(i).slope;
                lowest_cell_index = i;
            }
        }
        const Cell &lowest_cell = vicinity.neighbors.at(lowest_cell_index);
        if(lowest_cell.slope < 0)
        {
            float moved_sediment = data_.at(index(cell.i, cell.j)) * quantity;
            data_.at(index(lowest_cell.i, lowest_cell.j)) += moved_sediment;
            data_.at(index(cell.i, cell.j)) -= moved_sediment;
        }
    }
}

void HeightField::stream_power_erosion(float k, float n)
{
    std::vector<float> areas = stream_areas();
    for(unsigned int j = 0; j < ny_; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
            data_.at(index(i, j)) -= k * std::pow(areas.at(index(i, j)), 0.5f) * std::pow(slope(i, j), n);            
    }
}

void HeightField::road(unsigned int i, unsigned int j, unsigned int gi, unsigned int gj, int width,
    float slope_cost, float water_low_cost, float water_high_cost, float water_treshold)
{
    std::vector<Cell> path = shortest_path(i, j, gi, gj, slope_cost, water_low_cost, water_high_cost, water_treshold);
    for(const Cell &cell : path)
    {
        for(int j = -width; j <= width; ++j)
        {
            for(int i = -width; i <= width; ++i)
            {
                int sni = cell.i + i;
                int snj = cell.j + j;
                if(sni >= 0 && sni < (int)nx_ && snj >= 0 && snj < (int)ny_)
                {
                    data_.at(index(sni, snj)) = -0.0005f + value(sni, snj) + water_.at(index(sni, snj)); // TODO : improve
                    water_.at(index(sni, snj)) = 0.0f;
                }
            }
        }
    }
}

void HeightField::blur(unsigned int size)
{
    std::vector<float> tmp;
    tmp.reserve(nx_ * ny_);
    for(unsigned int j = 0; j < ny_; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
        {
            unsigned int nb = 0;
            float total = 0.0f;
            for(int jk = -size; jk <= (int)size; ++jk)
            {
                int snj = j + jk;
                if(snj >= 0 && snj < (int)ny_)
                {
                    for(int ik = -size; ik <= (int)size; ++ik)
                    {
                        int sni = i + ik;
                        if(sni >= 0 && sni < (int)nx_)
                        {
                            ++nb;
                            total += value((unsigned int)sni, (unsigned int)snj);
                        }
                    }
                }
            }
            tmp.push_back(total / nb);
        }
    }
    data_ = tmp;
}

void HeightField::fill(float height)
{
    for(unsigned int i = 0; i < nx_ * ny_; ++i)
    {
        if(data_.at(i) < height)
            water_.at(i) = height - data_.at(i);
    }
}

void HeightField::export_stream_areas(const std::string &path) const
{
    std::vector<float> areas = stream_areas();
    for(int i = 0; i < areas.size(); ++i)
        areas.at(i) = sqrt(areas.at(i));
    image_io::write_gray(path, areas, nx_, ny_);
}

void HeightField::export_wetness(const std::string &path) const
{
    std::vector<float> areas = stream_areas();
    for(unsigned int j = 0; j < ny_; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
            areas.at(index(i, j)) = sqrt(log(areas.at(index(i, j)) / (slope(i, j) + 0.00001f)));
    }
    image_io::write_gray(path, areas, nx_, ny_);
}

void HeightField::export_texture(const std::string &path) const
{
    std::vector<Color> colors;
    std::vector<float> areas = stream_areas();

    colors.reserve(nx_ * ny_);

    for(unsigned int j = 0; j < ny_; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
        {
            Color water_color = (Color(0.5f, 0.6f, 1.0f));
            Color rock_color = Color(0.9f, 0.7f, 0.3f) * slope(i, j);
            Color vegetation_color = Color(0.4f, 0.8f, 0.2f) * (1.0f - slope(i, j));
            Color final_color = (vegetation_color + rock_color) - log(areas.at(index(i, j)) / (slope(i, j) + 0.00001f)) / 10.0f;
            final_color = mix(final_color, water_color, water_.at(index(i, j)) * 50.0f);
            colors.push_back(final_color);
        }
    }
    image_io::write_color(path, colors, nx_, ny_);
}

Vector3<float> HeightField::normal(unsigned int i, unsigned int j) const
{
    Vector2<float> grad = gradient(i, j);
    return normalize(Vector3<float>(-grad.x, -grad.y, 1.0f));
}

Vector3<float> HeightField::point(unsigned int i, unsigned int j) const
{
    return Vector3<float>(i * scale_x_, j * scale_y_, value(i, j));
}

std::vector<HeightField::Cell> HeightField::sorted_cells() const
{
    std::vector<Cell> cells;
    cells.reserve(nx_ * ny_);
    
    for(unsigned int j = 0; j < ny_; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
            cells.push_back({i, j, value(i, j), 0.0f});
    }

    std::sort(cells.begin(), cells.end(), 
        [](const Cell &lhs, const Cell &rhs)
        {
            return lhs.height > rhs.height;
        }
    );

    return cells;
}

std::vector<float> HeightField::stream_areas() const
{
    std::vector<float> areas(nx_ * ny_, 1.0f);
    std::vector<Cell> cells = sorted_cells();
    
    for(const Cell &cell : cells)
    {
        Vicinity vicinity = vicinity_M18(cell);
        float total_slope = 0.0f;
        for(unsigned int i = 0; i < vicinity.neighbors.size(); ++i)
        {
            if(vicinity.neighbors.at(i).slope < 0.0f)
                total_slope += vicinity.neighbors.at(i).slope;
        }
        if(total_slope != 0.0f)
        {
            for(unsigned int i = 0; i < vicinity.neighbors.size(); ++i)
            {
                const Cell &n = vicinity.neighbors.at(i);
                if(n.slope < 0.0f)
                    areas.at(index(n.i, n.j)) += areas.at(index(cell.i, cell.j)) * (n.slope / total_slope);
            }
        }
    }
    return areas;
}

float HeightField::directional_slope(unsigned int i, unsigned int j, unsigned int ni, unsigned int nj) const
{
    float distance = length(Vector2<float>(i * scale_x_, j * scale_y_) - Vector2<float>(ni * scale_x_, nj * scale_y_));
    return (value(ni, nj) - value(i, j)) / distance;
}

HeightField::Vicinity HeightField::vicinity_M14(const Cell &cell) const
{
    Vicinity vicinity;
    vicinity.neighbors.reserve(4);
    for(int j = -1; j <= 1; ++j)
    {
        for(int i = -1; i <= 1; ++i)
        {
            if((i != 0 || j != 0) && (std::abs(i) + std::abs(j)) != 2)
            {
                int sni = cell.i + i;
                int snj = cell.j + j;
                if(sni >= 0 && sni < (int)nx_ && snj >= 0 && snj < (int)ny_)
                {
                    unsigned int ni = (unsigned int)sni;
                    unsigned int nj = (unsigned int)snj;
                    Cell n_cell = {ni, nj, value(ni, nj), directional_slope(cell.i, cell.j, ni, nj)};
                    vicinity.neighbors.push_back(n_cell);
                }
            }
        }
    }
    return vicinity;
}

HeightField::Vicinity HeightField::vicinity_M18(const Cell &cell) const
{
    Vicinity vicinity;
    vicinity.neighbors.reserve(8);
    for(int j = -1; j <= 1; ++j)
    {
        for(int i = -1; i <= 1; ++i)
        {
            if(i != 0 || j != 0)
            {
                int sni = cell.i + i;
                int snj = cell.j + j;
                if(sni >= 0 && sni < (int)nx_ && snj >= 0 && snj < (int)ny_)
                {
                    unsigned int ni = (unsigned int)sni;
                    unsigned int nj = (unsigned int)snj;
                    Cell n_cell = {ni, nj, value(ni, nj), directional_slope(cell.i, cell.j, ni, nj)};
                    vicinity.neighbors.push_back(n_cell);
                }
            }
        }
    }
    return vicinity;
}

HeightField::Vicinity HeightField::vicinity_M2(const Cell &cell) const
{
    Vicinity vicinity;
    vicinity.neighbors.reserve(16);
    for(int j = -2; j <= 2; ++j)
    {
        for(int i = -2; i <= 2; ++i)
        {
            if(i != 0 || j != 0)
            {
                int sni = cell.i + i;
                int snj = cell.j + j;
                if(sni >= 0 && sni < (int)nx_ && snj >= 0 && snj < (int)ny_)
                {
                    unsigned int ni = (unsigned int)sni;
                    unsigned int nj = (unsigned int)snj;
                    Cell n_cell = {ni, nj, value(ni, nj), directional_slope(cell.i, cell.j, ni, nj)};
                    if(abs(i) <= 1 && abs(j) <= 1)
                        vicinity.neighbors.push_back(n_cell);
                    else if(abs(i) + abs(j) == 3)
                        vicinity.neighbors.push_back(n_cell);
                }
            }
        }
    }
    return vicinity;
}

HeightField::Vicinity HeightField::vicinity_M3(const Cell &cell) const
{
    Vicinity vicinity;
    vicinity.neighbors.reserve(32);
    for(int j = -3; j <= 3; ++j)
    {
        for(int i = -3; i <= 3; ++i)
        {
            if(i != 0 || j != 0)
            {
                int sni = cell.i + i;
                int snj = cell.j + j;
                if(sni >= 0 && sni < (int)nx_ && snj >= 0 && snj < (int)ny_)
                {
                    unsigned int ni = (unsigned int)sni;
                    unsigned int nj = (unsigned int)snj;
                    Cell n_cell = {ni, nj, value(ni, nj), directional_slope(cell.i, cell.j, ni, nj)};
                    if(abs(i) <= 1 && abs(j) <= 1)
                        vicinity.neighbors.push_back(n_cell);
                    else if((abs(i) == 2 || abs(j) == 2) && (abs(i) + abs(j) == 3))
                        vicinity.neighbors.push_back(n_cell);
                    else if((abs(i) == 3 || abs(j) == 3) && ((abs(i) + abs(j) == 4) || (abs(i) + abs(j) == 5)))
                        vicinity.neighbors.push_back(n_cell);
                }
            }
        }
    }
    return vicinity;
}

DijkstraAdjacencyList HeightField::create_graph(float slope_cost, float water_low_cost, float water_high_cost, float water_treshold) const
{
    DijkstraAdjacencyList list(nx_ * ny_);
    for(unsigned int j = 0; j < ny_; ++j)
    {
        for(unsigned int i = 0; i < nx_; ++i)
        {
            Cell cell = {i, j, value(i, j), slope(i, j)};
            Vicinity vicinity = vicinity_M2(cell);
            for(unsigned int k = 0; k < vicinity.neighbors.size(); ++k)
            {
                const Cell &neighbor = vicinity.neighbors.at(k);
                float distance = length(point(neighbor.i, neighbor.j) - point(i, j));
                float neighbor_slope = std::abs(slope(neighbor.i, neighbor.j));
                float water;
                if(water_.at(index(i, j)) < water_treshold)
                    water = water_.at(index(i, j)) * water_low_cost;
                else
                    water = water_.at(index(i, j)) * water_high_cost;

                float cost = distance * (1 + (neighbor_slope * slope_cost) + water);
                list.at(index(i, j)).push_back(DijkstraNeighbor(index(neighbor.i, neighbor.j), cost));
            }
        }
    }
    return list;
}

std::vector<HeightField::Cell> HeightField::shortest_path(unsigned int i, unsigned int j, unsigned int gi, unsigned int gj,
    float slope_cost, float water_low_cost, float water_high_cost, float water_treshold) const
{
    std::vector<Cell> cells;
    DijkstraAdjacencyList adjacency_list = create_graph(slope_cost, water_low_cost, water_high_cost, water_treshold);
    std::vector<DijkstraWeight> min_distance;
    std::vector<DijkstraVertex> previous;
    compute_paths(index(i, j), adjacency_list, min_distance, previous);
    std::list<DijkstraVertex> path = get_shortest_path_to(index(gi, gj), previous);
    cells.reserve(path.size());
    for(const DijkstraVertex &vertex : path)
    {
        std::pair<unsigned int, unsigned int> ij = coords(vertex);
        Cell cell = {ij.first, ij.second, value(ij.first, ij.second), slope(ij.first, ij.second)};
        cells.push_back(cell);
    }
    return cells;
}