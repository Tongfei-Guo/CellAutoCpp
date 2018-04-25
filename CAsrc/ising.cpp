#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include <iostream>
#include <random>
#include <chrono>

//render:
#include <thread>
#include <CArender.hpp>

#include <iostream>
int main()
{
	auto process1 = process_type([](const grid_type &grid, Cell *self)
	{
		auto coord = get_coord(grid, self);
		unsigned x = coord.first, y = coord.second;
		std::vector<Cell*> neighbors = get_neighbors(grid, x, y);
               
		
               //std::cout<<"selected grid: "<<x<<","<<y<<std::endl;
 
		float beta = 10;
		float J = 0.2;
                float mu = 0.005;
                float H = 1;
		state_value currentori = (*self)["ori"];
		state_value flipedori = (*self)["ori"] * -1;

                //std::cout<<"neighbor size: "<<neighbors.size()<<std::endl;
 
		float Hnew = 0, Hold = 0;
		for(auto adjescent : neighbors)
		{
                    if(adjescent == nullptr) continue;
                    //compute new energy
                    Hnew += -J*(*adjescent)["ori"]*flipedori - mu*H*flipedori;
		    //compute odd energy
                    Hold += -J*(*adjescent)["ori"]*currentori - mu*H*currentori;
		}

                   

		float accpetance = std::exp(-beta * (Hnew - Hold));// std::cout<<"accpetance: "<<accpetance<<std::endl;
		accpetance = accpetance > 1 ? 1 : accpetance;

      //          std::cout<<"accptance: "<<accpetance<<std::endl;

		if( (double) ((double) rand() / (RAND_MAX))< accpetance )  //do flip
			(*self)["ori"] *= -1;

	});

        auto process2 = process_type([](const grid_type &grid, Cell *self)
	{
		auto coord = get_coord(grid, self);
		unsigned x = coord.first, y = coord.second;
		std::vector<Cell*> neighbors = get_neighbors(grid, x, y);
               
		
               //std::cout<<"selected grid: "<<x<<","<<y<<std::endl;
 
		float beta = 10;
		float J = 0.2;
                float mu = 0.1;
                float H = -1;
		state_value currentori = (*self)["ori"];
		state_value flipedori = (*self)["ori"] * -1;

                //std::cout<<"neighbor size: "<<neighbors.size()<<std::endl;
 
		float Hnew = 0, Hold = 0;
		for(auto adjescent : neighbors)
		{
                    if(adjescent == nullptr) continue;
                    //compute new energy
                    Hnew += -J*(*adjescent)["ori"]*flipedori - mu*H*flipedori;
		    //compute odd energy
                    Hold += -J*(*adjescent)["ori"]*currentori - mu*H*currentori;
		}

                   

		float accpetance = std::exp(-beta * (Hnew - Hold));// std::cout<<"accpetance: "<<accpetance<<std::endl;
		accpetance = accpetance > 1 ? 1 : accpetance;

      //          std::cout<<"accptance: "<<accpetance<<std::endl;

		if( (double) ((double) rand() / (RAND_MAX))< accpetance )  //do flip
			(*self)["ori"] *= -1;

	});


	auto reset = reset_type([](Cell *self)
	{
		;//empty //(*self)["wasOpen"] = (*self)["open"];
	});
	auto init = init_type([](Cell *self)
	{
		(*self)["ori"] = (((double) rand() / (RAND_MAX)) > 0.5) ? 1:-1; //  (((double) rand() / (RAND_MAX)) > 0.4);
	});
	auto getcolor = getcolor_type([](Cell *self)
	{
		if((*self)["ori"] == 1)
			return 0;
		else
			return 1;
	});


    std::vector<bitcolor> palette = {
    		{255,255,255,255},
    		{0,0,0,255}
    };

    int gridsize = 500;

    Model model(world_param_type(gridsize, gridsize, 6), { grid_param_type("electron", 100, process1, reset, init) },1, getcolor);
    CAWorld world(model);
    //world.save2file("isinglog.txt");
    
    using namespace std::chrono_literals;
    CARender render(gridsize,gridsize,palette);
    //world.step(1);
    auto flipgen = std::bind(std::uniform_int_distribution<>{0,gridsize - 1},std::default_random_engine{});
    
    auto bitmap = world.print_world();


/*    for(int i = 0; i < gridsize*gridsize*5; i++)//render.Renderworld(bitmap))
    {
        int gridx = flipgen();
        int gridy = flipgen();
        world.step(gridx,gridy);
        //world.forall_step(1);
        //std::cout<<"selected grid: "<<gridx<<","<<gridy<<std::endl;
        if(i%4000 == 0)
	{
        	bitmap = world.print_world();
        	if(!render.Renderworld(bitmap)) break;
	}
        if(i%(gridsize*gridsize) == 0)
        {
            std::cout<<"overall flip:"<<i/(gridsize*gridsize)<<std::endl;
        }
        //std::this_thread::sleep_for(1000ms);
    }*/

    
    /*char tmpfile[] = "isinglog.txt";  
    world.save2file(tmpfile);
    
    Model newmodel(world_param_type(gridsize, gridsize, 6), { grid_param_type("electron", 100, process2, reset, init) },1, getcolor);
    CAWorld newworld(newmodel);
    newworld.loadfromfile(tmpfile);

    for(int i = 0; i < gridsize*gridsize*5; i++)//render.Renderworld(bitmap))
    {
        int gridx = flipgen();
        int gridy = flipgen();
        newworld.step(gridx,gridy);

        if(i%4000 == 0)
	{
        	auto newbitmap = newworld.print_world();
        	if(!render.Renderworld(newbitmap)) break;
	}
        if(i%(gridsize*gridsize) == 0)
        {
            std::cout<<"overall flip:"<<i/(gridsize*gridsize)<<std::endl;
        }
    }*/
    
    //measure speed


   auto start = std::chrono::high_resolution_clock::now();


    for(int i = 0; i < 10000; i++)//render.Renderworld(bitmap))
    {
        int gridx = flipgen();
        int gridy = flipgen();
        world.step(gridx,gridy);
        //world.forall_step(1);
        //std::cout<<"selected grid: "<<gridx<<","<<gridy<<std::endl;
        /*if(i%4000 == 0)
	{
        	bitmap = world.print_world();
        	if(!render.Renderworld(bitmap)) break;
	}
        if(i%(gridsize*gridsize) == 0)
        {
            std::cout<<"overall flip:"<<i/(gridsize*gridsize)<<std::endl;
        }*/

        //std::this_thread::sleep_for(1000ms);

    }


    //world3.forall_step(100);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::cout << nanoseconds << "nanoseconds\n";


}

