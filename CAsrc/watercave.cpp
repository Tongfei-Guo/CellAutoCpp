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

//render:
#include <thread>
#include <CArender.hpp>

#include <iostream>

//generate a cave filled with water
//step 1: generate a vaccum cave
//step 2: fil the cave with water
int main()
{
    std::vector<bitcolor> palette = {
    		{89, 125, 206, 0},         //0
    		{89, 125, 206, 255/9},     //1
                {89, 125, 206, 2*255/9},   //2
                {89, 125, 206, 3*255/9},   //3
                {89, 125, 206, 4*255/9},   //4
                {89, 125, 206, 5*255/9},   //5
                {89, 125, 206, 6*255/9},   //6
                {89, 125, 206, 7*255/9},   //7
                {89, 125, 206, 8*255/9},   //8  
                {255,255,255,255},         //9
                {0,0,0,255}                //10
    };

    int gridsize = 100;
    CARender render(gridsize,gridsize,palette);
    using namespace std::chrono_literals;

    //step 1. generate a vaccum cave
    auto vaccumcave_process = process_type([] (const grid_type &grid, Cell *self)
    {
        std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
        int surrounding = countSurroundingCellsWithValue(neighbors, "wasOpen");
        (*grid[self->x][self->y])["open"] = ((*grid[x][y])["wasOpen"] && surrounding >=4 || surrounding >= 6);
    });
    auto vaccumcave_reset = reset_type([](Cell *self)
    {
        (*self)["wasOpen"] = (*self)["open"];
    });
    auto vaccumcave_init = init_type([](Cell *self)
    {
        (*self)["open"] = (((double) rand() / (RAND_MAX)) > 0.4);
    });

    auto vaccum_getcolor = getcolor_type([](Cell *self)
    {
        if((*self)["open"] == 1)
	    return 9;
	else
	    return 10;
    });

    CAWorld vaccumcave(Model(world_param_type(gridsize, gridsize, 6), { grid_param_type("Wall", 100, vaccumcave_process, vaccumcave_reset, vaccumcave_init) },0,vaccum_getcolor));
    vaccumcave.forall_step(10);


    auto bitmap = vaccumcave.print_world();
    if(!render.Renderworld(bitmap)) std::cout<<"done"<<std::endl;//break;


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
    	auto coord = get_coord(grid, self);
        unsigned x = coord.first, y = coord.second;
        std::vector<Cell*> neighbors = get_neighbors(grid, x, y);

        int thislevel = (*self)["level"];
        if(thislevel == 0)
        	return;


        // cell below me will take all it can
        if(neighbors[CAWorld::BOTTOM] != nullptr &&
        	thislevel!=0 &&
		   neighbors[CAWorld::BOTTOM]->get_type()=="water" && (*neighbors[CAWorld::BOTTOM])["level"] < 9 )
        {
        	int amt = std::min(thislevel,9 - (*neighbors[CAWorld::BOTTOM])["level"]);
            (*self)["level"] -= amt;
            (*neighbors[CAWorld::BOTTOM])["level"] += amt;
        	return ;
        }

        // bottom two corners take half of what I have
        for(int i = 5; i < 8; i++)
        {
        	if( i!=CAWorld::BOTTOM &&
        		neighbors[i] != nullptr &&
				thislevel != 0 &&
				neighbors[i]->get_type()=="water" && (*neighbors[i])["level"] < 9)
        	{
                int amt = std::min(thislevel, (int)std::ceil( (9.0 - (*neighbors[i])["level"])/2.0 ) );
                (*self)["level"] -= amt;
                (*neighbors[i])["level"] += amt;
                return;
        	}
        }

        // sides take a third of what I have
        for(int i = 3; i < 5; i++)
        {
        	if (neighbors[i] != nullptr &&
        		neighbors[i]->get_type()=="water" && (*neighbors[i])["level"]  < thislevel)
        	{
        		int amt = std::min(thislevel, (int)std::ceil( (9.0 - (*neighbors[i])["level"])/3.0 ) );
        		(*self)["level"] -= amt;
        		(*neighbors[i])["level"] += amt;
        		return ;
        	}
        }

    });


    auto water_init = init_type([](Cell *self)
    {
        (*self)["level"] = (int)std::floor( ((double) rand() / (RAND_MAX)) * 9 ); //the cave cell must be blocked
        //std::cout<<"waterlevel:"<<(*self)["level"]<<std::endl;
    });

    auto fullcave_getcolor = getcolor_type([](Cell *self)
    {
        
        if(self->get_type()=="Wall")
        {  
            return 10;
        }
        else
        {
          //std::cout<<"water level"<<(*self)["levl"]<<std::endl;
          return (*self)["level"];
        } 
    });



    Model watercavemodel(world_param_type(gridsize, gridsize, 6),
    		             { grid_param_type("Wall", 20, fullcave_processs, {}, fullcave_init) ,
    		               grid_param_type("water",80, water_processs, {}, water_init)},1,fullcave_getcolor);
    CAWorld fullcave(watercavemodel);
    fullcave.initgridfromgridref(cavemap);

    //fullcave.forall_step(1);

    bitmap = fullcave.print_world();
    for(int i = 0; i < 10000; i ++)
    {
        fullcave.forall_step(1);
        bitmap = fullcave.print_world();
        if(!render.Renderworld(bitmap)) break;
    }
    
    std::this_thread::sleep_for(2000ms);
    std::cout<<"end of program"<<std::endl;
	return 0;
}
