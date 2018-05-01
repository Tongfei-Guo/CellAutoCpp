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

    std::vector<int> rullset{0, 1, 0, 0, 1, 0, 0, 1};
    auto process = process_type([&rullset] (const grid_type &grid, Cell *self)
    {
        std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
        if ((*self)["age"] == self->y){
            int v=0;
            bool t=0;
            for(auto &c: neighbors)
                if (c!=NULL && c->y == self->y - 1){
                    v=v*2+(*c)["alive"];
                    t=1;
                }
            (*self)["alive"]=(t)?(rullset[v]):(!self->x);
        }
        (*self)["age"]++;
    });
    auto reset = reset_type();
    auto init = init_type([](Cell *self)
    {
        (*self)["alive"] = 0;
        (*self)["age"] = 0;
    });
    auto getcolor = getcolor_type([](Cell *self)
    {
    	return (*self)["alive"];
    });


    std::vector<bitcolor> palette = {
        {0,0,0,255},
        {0,100,0,255}
    };
    
    CARender render(200,200, palette);
    using namespace std::chrono_literals;


    CAWorld world(Model(world_param_type(200, 200, 6), { grid_param_type("elementary", 100, process, reset, init) },1,getcolor));
    for(int i = 0; i < 1000; i++)
    {
        world.forall_step(1);
        auto bitmap = world.print_world();
        if(!render.Renderworld(bitmap)) break;
    }


    
	return 0;
}
