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

    std::vector<bitcolor> palette = {
    		{255,255,255,255},
    		{0,0,0,255}
    };

	Model model(world_param_type(50, 50, 6), palette, { grid_param_type("Wall", 100, process, reset, init, getcolor) });
    CAWorld world(model);
	auto start = std::chrono::high_resolution_clock::now();
    world.step(100);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::cout << nanoseconds << "nanoseconds\n";
    world.print_test();
	return 0;
}

