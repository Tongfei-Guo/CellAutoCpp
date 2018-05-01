//test purpose, don't delete until production
#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include "CAFunctions.h"
#include <chrono>
#include <iostream>
#include "CAConsoleViz.h"
int main()
{
    auto process = process_type([] (const grid_type &grid, Cell *self)
    {
        auto neighbors = get_neighbors(grid, self->x, self->y);
        auto cnt=countSurroundingCellsWithValue(neighbors, "wasAlive");
        (*self)["alive"]=(cnt==3 || (cnt==2 && (*self)["alive"]));
    });
    auto reset = reset_type([](Cell *self)
    {
        (*self)["wasAlive"]=(*self)["alive"];
    });
    auto init = init_type([](Cell *self)
    {
        (*self)["alive"] = (rand()>RAND_MAX/2);
    });
    auto getcolor = getcolor_type([](Cell *self)
    {
    	if ((*self)["alive"]==0)
            return 1;
        else
            return 0;
    });

/*
    std::vector<bitcolor> palette = {
    {68, 36, 52, 255},
    {255, 255, 255, 255}
    };
    */

    CAWorld world(Model(world_param_type(60, 60, 6), { grid_param_type("living", 100, process, reset, init) },1, getcolor));

    CAConsoleViz myviz(&world);

    myviz.SetColorScheme(
        [](Cell *cell)->char{
            if ((*cell)["alive"]==1)
                return 'O';
            else
                return '.';
        }
    );

    myviz.Visualize(1000,10 );
	return 0;
}
