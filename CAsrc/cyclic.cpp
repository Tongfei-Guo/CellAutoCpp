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
    
    CARender render(50,50,palette);
    using namespace std::chrono_literals;

    auto process = process_type([] (const grid_type &grid, Cell *self)
    {
        std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
        int next=((*grid[self->x][self->y])["state"]+rand()%2)%16;
        bool changing=false;
        for(auto n: neighbors)
            if (n!=NULL)
                changing=changing||((*n)["state"]==next);
        if (changing)
            (*grid[self->x][self->y])["state"]=next;
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
    CAWorld world(Model(world_param_type(50, 50, 6), { grid_param_type("Cyclic", 100, process, reset, init) },1));
    //world.AddMeasure(new CADistributionMeasure());
	auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 100; i++)
    {
        world.forall_step(1);
        auto bitmap = world.print_world();
        if(!render.Renderworld(bitmap)) std::cout<<"done"<<std::endl;//break;
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
