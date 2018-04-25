#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include <iostream>
#include <random>

//render:
#include <thread>
#include <CArender.hpp>

#include <iostream>

int main()
{
    auto process = process_type([] (const grid_type &grid, Cell *self)
    {
        auto coord = get_coord(grid, self);
        unsigned x = coord.first, y = coord.second;
        std::vector<Cell*> neighbors = get_neighbors(grid, x, y);
        int surrounding = countSurroundingCellsWithValue(neighbors, "wasOpen");
        (*grid[x][y])["open"] = ((*grid[x][y])["wasOpen"] && surrounding >=4 || surrounding >= 6);
    });
    auto reset = reset_type([](Cell *self)
    {
        (*self)["wasOpen"] = (*self)["open"];
    });
    auto init = init_type([](Cell *self)
    {
        (*self)["open"] = (((double) rand() / (RAND_MAX)) > 0.4);
    });

    CAWorld emptycave(Model(world_param_type(100, 100, 6), { grid_param_type("Wall", 100, process, reset, init) },0));
 
	auto start = std::chrono::high_resolution_clock::now();
	//world3.combine(world1.forall_step(2), 0, 99, 0, 49);
    world3.combine(world1.forall_step(2), 0, 100, 0, 50).combine(world2.forall_step(1), 0, 100, 50, 100).forall_step(1);
    //world3.forall_step(100);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::cout << nanoseconds << "nanoseconds\n";
    auto timestamp = world3.get_timestamps();
    world3.print_test(timestamp, 0);
	return 0;
}
