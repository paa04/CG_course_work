//
// Created by paa04 on 16.12.24.
//

#include "experement.h"

#include "Terrain.h"
#include "Terrain_manager.h"

void experement::do_exp()
{
    setbuf(stdout, 0);
    for (int n = 2; n < 15; ++n)
    {
        Terrain tmp;
        clock_t t = clock();
        for (int k = 0; k < 5; ++k)
        {
            Terrain_manager::gen_height_map(n, tmp, 0.4);
        }
        t = clock() - t;

        cout << n << " " << t / 5 << endl;
    }
}
