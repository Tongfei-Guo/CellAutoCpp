#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"


//render:
#include <thread>
#include <CArender.hpp>

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

    Model model(world_param_type(50, 50, 6), { grid_param_type("Wall", 100, process, reset, init, getcolor) });
    CAWorld world(model);
	world.step(10);
    
    using namespace std::chrono_literals;
    CARender render(50,50,palette);
    auto bitmap = world.print_world();
    std::cout<<bitmap.size()<<std::endl;
    while (render.Renderworld(bitmap))
    {
        std::this_thread::sleep_for(10ms);
    }
}

