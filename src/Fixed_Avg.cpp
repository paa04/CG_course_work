//
// Created by paa04 on 30.10.24.
//

#include "Fixed_Avg.h"

using namespace std;

double Fixed_Avg::avg(const Terrain &terrain, size_t row, size_t col, size_t neighbor_dist,
                      std::vector<std::pair<int, int> > offset)
{
    int size = terrain.size();

    double res = 0, k = 0;

    for (const auto &par: offset)
    {
        int p = par.first, q = par.second;
        int pp = row + p * neighbor_dist;
        int qq = col + q * neighbor_dist;
        if (pp >= 0 && pp < size && qq >= 0 && qq < size)
        {
            res += terrain[pp][qq];
            ++k;
        }
    }

    return res / k;
}
