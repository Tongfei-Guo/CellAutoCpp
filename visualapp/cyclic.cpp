//test purpose, don't delete until production
#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include "CAFunctions.h"
#include <chrono>
#include <iostream>

//render:
#include <thread>
#include <CArender.hpp>

#include <iostream>


int main()
{

    std::vector<bitcolor> palette = {
    {255,0,0,255}, {255,96,0,255}, {255,191,0,255}, {223,255,0,255},
    {128,255,0,255}, {32,255,0,255}, {0,255,64,255}, {0,255,159,255},
    {0,255,255,255}, {0,159,255,255}, {0,64,255,255}, {32,0,255,255},
    {127,0,255,255}, {223,0,255,255}, {255,0,191,255}, {255,0,96,255}
    };
    
    CARender render(100,100,palette);
    using namespace std::chrono_literals;

    auto process = process_type([] (const grid_type &grid, Cell *self)
    {
        std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
        int next=((*self)["state"]+1)%16;
        bool changing=false;
        for(auto n: neighbors)
            if (n!=NULL)
                changing=changing||((*n)["state"]==next);
        if (changing)
            (*self)["state"]=next;
    });
    auto reset = reset_type();
    auto init = init_type([](Cell *self)
    {
        (*self)["state"] = rand()%16;
    });
    auto getcolor = getcolor_type([](Cell *self)
    {
    	return (*self)["state"];
    });

    //CAWorld world1(Model(world_param_type(100, 50, 6), { grid_param_type("Wall", 100, process, reset, init) },0));
    //CAWorld world2(Model(world_param_type(100, 50, 6), { grid_param_type("Wall", 100, process, reset, init) },1));
    CAWorld world(Model(world_param_type(100, 100, 6), { grid_param_type("Cyclic", 100, process, reset, init) },1,getcolor));
    //world.AddMeasure(new CADistributionMeasure());
	auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 1000; i++)
    {
        world.forall_step(1);
        auto bitmap = world.print_world();
        if(!render.Renderworld(bitmap)) break;
    }

	
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::cout << nanoseconds << "nanoseconds\n";
    auto timestamp = world.get_timestamps();
    world.print_test(timestamp, 0);
    world.AddMeasureAndRun(new CADistributionMeasure("Final Dist"));

    for(auto m: world.GetMeasures())
        std::cout<<"========="<<m->GetName()<<"========="<<std::endl<<m->Str_All()<<std::endl;

	return 0;
}
