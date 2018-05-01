//test purpose, don't delete until production
#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include "CAFunctions.h"
#include <chrono>
#include <iostream>
#include <cmath>


//generate a cave filled with water
//step 1: generate a vaccum cave
//step 2: fil the cave with water
int cavewatermain()
{


    //step 1. generate a vaccum cave
    auto vaccumcave_process = process_type([] (const grid_type &grid, Cell *self)
    {
        std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
        int surrounding = countSurroundingCellsWithValue(neighbors, "wasOpen");
        (*grid[self->x][self->y])["open"] = ((*grid[self->x][self->y])["wasOpen"] && surrounding >=4 || surrounding >= 6);
    });
    auto vaccumcave_reset = reset_type([](Cell *self)
    {
        (*self)["wasOpen"] = (*self)["open"];
    });
    auto vaccumcave_init = init_type([](Cell *self)
    {
        (*self)["open"] = (((double) rand() / (RAND_MAX)) > 0.4);
    });

    CAWorld vaccumcave(Model(world_param_type(50, 50, 6), { grid_param_type("Wall", 100, vaccumcave_process, vaccumcave_reset, vaccumcave_init) },0));
    vaccumcave.forall_step(10);

    auto gridref = gettypeind_type([](Cell *self)
    {
        if((*self)["open"] == 1)
    	    return std::string("water");
        else
            return std::string("Wall"); });

    auto cavemap = vaccumcave.getgridref(gridref);

    //step 2.fill the cave with water
    auto fullcave_processs = process_type([](const grid_type &grid, Cell *self)
    {
        ; //do nothing
    });
    auto fullcave_init = init_type([](Cell *self)
    {
        (*self)["open"] = 0; //the cave cell must be blocked
    });

    //water cell behave
    //water cell has 9 water level. Higher level indicates more water in the cell.
    auto water_processs = process_type([](const grid_type &grid, Cell *self)
    {
    	//auto coord = get_coord(grid, self);
        unsigned x = self->x, y = self->y;

    	std::vector<Cell*> neighbors = get_neighbors(grid, x, y);

        int thislevel = (*self)["level"];
        if(thislevel == 0)
        	return;


        // cell below me will take all it can
        if(neighbors[BOTTOM] != nullptr &&
        	thislevel!=0 &&
		   neighbors[BOTTOM]->get_type()=="water" && (*neighbors[BOTTOM])["level"] < 9 )
        {
        	int amt = std::min(thislevel,9 - (*neighbors[BOTTOM])["level"]);
            (*self)["level"] -= amt;
            (*neighbors[BOTTOM])["level"] += amt;
        	return ;
        }

        // bottom two corners take half of what I have
        for(int i = 5; i < 8; i++)
        {
        	if( i!=BOTTOM &&
        		neighbors[i] != nullptr &&
				thislevel != 0 &&
				neighbors[i]->get_type()=="water" && (*neighbors[i])["level"] < 9)
        	{
                int amt = std::min(thislevel, (int)std::ceil( (9 - (*neighbors[i])["level"])/2 ) );
                (*self)["level"] -= amt;
                (*neighbors[i])["level"] += amt;
                return;
        	}
        }

        // sides take a third of what I have
        for(int i = 5; i < 8; i++)
        {
        	if (neighbors[i] != nullptr &&
        		neighbors[i]->get_type()=="water" && (*neighbors[i])["level"]  < thislevel)
        	{
        		int amt = std::min(thislevel, (int)std::ceil( (9 - (*neighbors[i])["level"])/3 ) );
        		(*self)["level"] -= amt;
        		(*neighbors[i])["level"] += amt;
        		return ;
        	}
        }

    });

    auto water_init = init_type([](Cell *self)
    {
        (*self)["level"] = (int)std::floor(((double) rand() / (RAND_MAX)) * 9); //the cave cell must be blocked
    });


    Model watercavemodel(world_param_type(50, 50, 6),
    		             { grid_param_type("Wall", 20, fullcave_processs, {}, fullcave_init) ,
    		               grid_param_type("water",80, water_processs, {}, water_init)});
    CAWorld fullcave(watercavemodel);
    fullcave.initgridfromgridref(cavemap);

    fullcave.forall_step(1);

    std::cout<<"end of program"<<std::endl;
	return 0;
}
