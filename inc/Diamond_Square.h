//
// Created by paa04 on 30.10.24.
//

#ifndef DIAMOND_SQUARE_H
#define DIAMOND_SQUARE_H

#include <memory>

#include "Avg_Strategy.h"
#include "Terrain.h"

class Diamond_Square
{
private:
    shared_ptr<Avg_Strategy> strategy;

    void diamond_step(Terrain& terrain, int step_wide, double noise);
    size_t n;
    double smooth;

    static double random(double noise);
    static void init_rand_dev();

    void diamond_step_row(Terrain &terrain, int step_wide, double noise);
    void diamond_step_col(Terrain &terrain, int step_wide, double noise);
public:
    Diamond_Square(size_t ter_size, double smooth, shared_ptr<Avg_Strategy> ptr):n(ter_size), smooth(smooth), strategy(ptr)
    {
        n = (1 << n) + 1;
        init_rand_dev();
    }
    Terrain generate_terrain();
    static void smoothHeightMap(Matrix<double> &heightMap);
};

#endif //DIAMOND_SQUARE_H
