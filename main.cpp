
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
        unsigned x = self->x, y = self->y;
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
    auto getcolor = getcolor_type([](Cell *self)
    {
    	std::cout<<"in getcolor"<<std::endl;
		if (self->get_type() == "Wall")
		{
			std::cout<<"in get_type()"<<std::endl;
			if((*self)["open"])
				return 0;
			else
				return 1;
		}
    });

    //CAWorld world1(Model(world_param_type(100, 50, 6), { grid_param_type("Wall", 100, process, reset, init) },0));
    //CAWorld world2(Model(world_param_type(100, 50, 6), { grid_param_type("Wall", 100, process, reset, init) },0));
    CAWorld world3(Model(world_param_type(50, 50, 6), { grid_param_type("Wall", 100, process, reset, init) },4));
    //world3.AddMeasure(new CADistributionMeasure());
    //world1.forall_step(2);
	//world3.combine(world1, 0, 100, 0, 50);
    //world3.combine(world1.forall_step(2), 0, 100, 0, 50).combine(world2.forall_step(1), 0, 100, 50, 100).forall_step(1);
    world3.forall_step(100);
    auto timestamp = world3.get_timestamps();
    world3.print_test(timestamp, 0);

	return 0;
}
