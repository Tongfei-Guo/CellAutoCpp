//test purpose, don't delete until production
#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include "CAFunctions.h"
#include <chrono>

//render:
#include <thread>
#include <CArender.hpp>

#include <iostream>

int main()
{

    std::vector<bitcolor> palette = {
    {68, 36, 52, 255},
    {255, 255, 255, 255}
    };
    
    CARender render(96, 64, palette);
    using namespace std::chrono_literals;   

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


    CAWorld world(Model(world_param_type(96, 64, 6), { grid_param_type("living", 100, process, reset, init) },1, getcolor));
    for(int i = 0; i < 1000; i++)
    {
        world.forall_step(1);
        auto bitmap = world.print_world();
        if(!render.Renderworld(bitmap)) break;
    }


	return 0;
}
