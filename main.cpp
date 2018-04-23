//test purpose, don't delete until production
#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include <chrono>
#include <iostream>
int main()
{
    auto process = process_type([](Cell *self, std::vector<Cell*> neighbors)
    {
        int surrounding = Cell::countSurroundingCellsWithValue(neighbors, "wasOpen");
        (*self)["open"] = ((*self)["wasOpen"] && surrounding >=4 || surrounding >= 6);
    });
    auto reset = reset_type([](Cell *self)
    {
        (*self)["wasOpen"] = (*self)["open"];
    });
    auto init = init_type([](Cell *self)
    {
        (*self)["open"] = (((double) rand() / (RAND_MAX)) > 0.4);
    });
     auto getcolor = getcolor_type([](Cell *self)
    {
    	if((*self)["open"])
    		return 0;
    	else
    		return 1;
    });

    CAWorld world1(Model(world_param_type(100, 50, 6), { grid_param_type("Wall", 100, process, reset, init, getcolor) },0));
    CAWorld world2(Model(world_param_type(100, 50, 6), { grid_param_type("Wall", 100, process, reset, init, getcolor) },1));
    CAWorld world3(Model(world_param_type(100, 100, 6), { grid_param_type("Wall", 100, process, reset, init, getcolor) },3));
	auto start = std::chrono::high_resolution_clock::now();
	//world3.combine(world1.step(2), 0, 99, 0, 49);
    world3.combine(world1.step(2), 0, 99, 0, 49).combine(world2.step(1), 0, 99, 50, 99).step(1);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::cout << nanoseconds << "nanoseconds\n";
    auto timestamp = world3.get_timestamps();
    world3.print_test(timestamp, 1);
	return 0;
}

