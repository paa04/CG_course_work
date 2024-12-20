//
// Created by paa04 on 30.10.24.
//

#ifndef AVG_STRATEGY_H
#define AVG_STRATEGY_H
#include <vector>

#include "Terrain.h"


class Avg_Strategy
{
public:
    virtual double avg(const Terrain &terrain, size_t row, size_t col, size_t neighborDist,
                       std::vector<std::pair<int, int> > offset) = 0;
    virtual ~Avg_Strategy() = default;
};


#endif //AVG_STRATEGY_H
