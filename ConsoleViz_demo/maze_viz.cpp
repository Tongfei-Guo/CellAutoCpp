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
        std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
        auto cnt=countSurroundingCellsWithValue(neighbors, "wasAlive");
        int &sim=(*self)["simulated"];
        if (sim<20)
        {
            (*self)["alive"] = (cnt==1 || (cnt==2 && (*self)["alive"]));
        }else if (sim>20 && cnt==2)
            (*self)["alive"] = 1;
        ++sim;
    });
    auto reset = reset_type([](Cell *self)
    {
        (*self)["wasAlive"] = (*self)["alive"];
    });
    auto init = init_type([](Cell *self)
    {
        (*self)["alive"] = (rand()>RAND_MAX/2);
        (*self)["simulated"] = 0;
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

    CAWorld world(Model(world_param_type(96, 64, 6), { grid_param_type("living", 100, process, reset, init) },1, getcolor));

    CAConsoleViz myviz(&world);

    myviz.SetColorScheme(
        [](Cell *cell)->char{

          //std::cout<<"water level"<<(*self)["levl"]<<std::endl;
          if ((*cell)["alive"]==1)
              return '#';
          else
              return ' ';
        }
    );

    myviz.Visualize(1000,10);

	return 0;
}
