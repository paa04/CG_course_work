//
// Created by paa04 on 30.10.24.
//

#ifndef FIXED_AVG_H
#define FIXED_AVG_H
#include "Avg_Strategy.h"

class Fixed_Avg : public Avg_Strategy
{
public:
    double avg(const Terrain &terrain, size_t row, size_t col, size_t neighbor_dist,
               std::vector<std::pair<int, int> > offset) override;
};


#endif //FIXED_AVG_H
