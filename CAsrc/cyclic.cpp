//test purpose, don't delete until production
#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include "CAFunctions.h"
#include <chrono>
#include <iostream>
int main()
{
    auto process = process_type([] (const grid_type &grid, Cell *self)
    {
        auto coord = get_coord(grid, self);
        unsigned x = coord.first, y = coord.second;
        std::vector<Cell*> neighbors = get_neighbors(grid, x, y);
        int next=((*grid[x][y])["state"]+rand()%2)%16;
        bool changing=false;
        for(auto n: neighbors)
            if (n!=NULL)
                changing=changing||((*n)["state"]==next);
        if (changing)
            (*grid[x][y])["state"]=next;
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
	world.forall_step(1000);
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
