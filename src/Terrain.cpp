//
// Created by paa04 on 30.10.24.
//

#include "Terrain.h"

std::ostream &operator<<(std::ostream &os, const Terrain &t)
{
    for (size_t i = 0; i < t.size(); ++i)
    {
        for (const auto &el: t[i])
        {
            os << el << " ";
        }
        os << endl;
    }

    return os;
}

void Terrain::clear()
{
    height_mtr.clear();
    screen_mtr.clear();
    normal_plains.clear();
    intensity.clear();
    min_height = max_height = 0;
}

// void Terrain::add_operation(shared_ptr<Operation> ptr)
// {
//     operations_history.push_back(ptr);
// }
//
// void Terrain::undo_operation()
// {
//     if (operations_history.size() > 0)
//     {
//         operations_history.rend()->get()->undo(*this);
//         auto del_pos = --operations_history.end();
//         operations_history.erase(del_pos);
//     }
// }
