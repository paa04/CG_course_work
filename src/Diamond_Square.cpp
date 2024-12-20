//
// Created by paa04 on 30.10.24.
//
#include "Diamond_Square.h"

#include <memory>
#include <random>

using namespace std;

static random_device rd;
static mt19937 random_generator;

void Diamond_Square::init_rand_dev()
{
    random_generator.seed(rd());
}


double Diamond_Square::random(double noise)
{
    uniform_real_distribution<double> distribution{-noise, noise};
    return distribution(random_generator);
}


void Diamond_Square::diamond_step(Terrain &terrain, int step_wide, double noise)
{
    int neighbour_dist = step_wide / 2;
    vector<pair<int, int> > diamond_offset = {{-1, -1}, {-1, 1}, {1, 1}, {1, -1}};

    for (size_t row = neighbour_dist; row < terrain.size(); row += step_wide)
    {
        for (size_t col = neighbour_dist; col < terrain.size(); col += step_wide)
        {
            terrain[row][col] = strategy->avg(terrain, row, col, neighbour_dist, diamond_offset) + random(noise);
            terrain.set_max_h(max(terrain.get_max_h(), terrain[row][col]));
            terrain.set_min_h(min(terrain.get_min_h(), terrain[row][col]));
        }
    }
}

void Diamond_Square::diamond_step_row(Terrain &terrain, int step_wide, double noise)
{
    int neighbour_dist = step_wide / 2;
    vector<pair<int, int> > square_offset = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

    for (size_t row = neighbour_dist; row < terrain.size(); row += step_wide)
    {
        for (size_t col = 0; col < terrain.size(); col += step_wide)
        {
            terrain[row][col] = strategy->avg(terrain, row, col, neighbour_dist, square_offset) + random(noise);
            terrain.set_max_h(max(terrain.get_max_h(), terrain[row][col]));
            terrain.set_min_h(min(terrain.get_min_h(), terrain[row][col]));
        }
    }
}

void Diamond_Square::diamond_step_col(Terrain &terrain, int step_wide, double noise)
{
    int neighbour_dist = step_wide / 2;
    vector<pair<int, int> > square_offset = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

    for (size_t row = 0; row < terrain.size(); row += step_wide)
    {
        for (size_t col = neighbour_dist; col < terrain.size(); col += step_wide)
        {
            terrain[row][col] = strategy->avg(terrain, row, col, neighbour_dist, square_offset) + random(noise);
            terrain.set_max_h(max(terrain.get_max_h(), terrain[row][col]));
            terrain.set_min_h(min(terrain.get_min_h(), terrain[row][col]));
        }
    }
}

Terrain Diamond_Square::generate_terrain()
{
    Terrain terrain(n);

    int step_wide = n;
    double rand_noise = 1;

    while (step_wide > 1)
    {
        diamond_step(terrain, step_wide, rand_noise);
        diamond_step_row(terrain, step_wide, rand_noise);
        diamond_step_col(terrain, step_wide, rand_noise);

        step_wide /= 2;
        rand_noise *= smooth;
    }

    return terrain;
}

void Diamond_Square::smoothHeightMap(Matrix<double> &heightMap)
{
    int width = heightMap.size();
    int height = heightMap[0].size();
    Matrix<double> smoothedHeightMap = heightMap;

    for (int i = 1; i < width - 1; ++i)
    {
        for (int j = 1; j < height - 1; ++j)
        {
            smoothedHeightMap[i][j] = (heightMap[i - 1][j] +
                                       heightMap[i + 1][j] +
                                       heightMap[i][j - 1] +
                                       heightMap[i][j + 1] +
                                       heightMap[i][j]) / 5.0;
        }
    }

    heightMap = smoothedHeightMap;
}
