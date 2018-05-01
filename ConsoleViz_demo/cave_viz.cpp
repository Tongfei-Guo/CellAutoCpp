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
        int surrounding = countSurroundingCellsWithValue(neighbors, "wasOpen");
        (*grid[self->x][self->y])["open"] = ((*grid[self->x][self->y])["wasOpen"] && surrounding >=4 || surrounding >= 6);
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
		if (self->get_type() == "Wall")
		{
			if((*self)["open"])
				return 0;
			else
				return 1;
		}
    });

    //CAWorld world1(Model(world_param_type(100, 50, 6), { grid_param_type("Wall", 100, process, reset, init) },0));
    //CAWorld world2(Model(world_param_type(100, 50, 6), { grid_param_type("Wall", 100, process, reset, init) },1));
    CAWorld world(Model(world_param_type(50, 50, 6), { grid_param_type("Wall", 100, process, reset, init) },1));

    CAConsoleViz myviz(&world);

    myviz.SetColorScheme(
        [](Cell *cell)->char{
            if((*cell)["open"])
				return '#';
			else
				return ' ';
        }
    );

    myviz.Visualize(1000,10);

	return 0;
}
