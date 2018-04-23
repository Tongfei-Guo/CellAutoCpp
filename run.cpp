#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
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
    	if (self->get_type() == "Wall")
		{
			if((*self)["open"])
				return 0;
			else
				return 1;
		}
    });


	Model model(world_param_type(50, 50, 6), { grid_param_type("Wall", 100, process, reset, init) },1,getcolor);
    CAWorld world(model);
	world.step(10);
    world.print_world();
	return 0;
}

