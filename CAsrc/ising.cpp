#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include <iostream>

//render:
#include <thread>
#include <CArender.hpp>

#include <iostream>
int main()
{
	auto process = process_type([](Cell *self, std::vector<Cell*> neighbors)
	{
		float beta = 0.8;
		float J = 2;
		state_value currentori = (*self)["ori"];
		state_value flipedori = (*self)["ori"] * -1;

		float Hnew = 0, Hold = 0;
		for(auto adjescent : neighbors)
		{
                    if(adjescent == nullptr) continue;
                    //compute new energy
                    Hnew += -J*(*adjescent)["ori"]*flipedori;
		    //compute odd energy
                    Hold += -J*(*adjescent)["ori"]*currentori;
		}
		float accpetance = std::exp(-beta * (Hnew - Hold));
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

    Model model(world_param_type(500, 500, 6), { grid_param_type("electron", 100, process, reset, init) },1, getcolor);
    CAWorld world(model);
    world.save2file("isinglog.txt");
    
    using namespace std::chrono_literals;
    CARender render(500,500,palette);
    //world.step(1);

    auto bitmap = world.print_world();
    render.Renderworld(bitmap);
    std::this_thread::sleep_for(500ms);    
   
    world.step(10);
    bitmap = world.print_world();
    render.Renderworld(bitmap);
    std::this_thread::sleep_for(500ms);  
   
    world.loadfromfile("isinglog.txt");
    bitmap = world.print_world();
    render.Renderworld(bitmap);
    std::this_thread::sleep_for(2000ms); 
    
    world.step(5);
    bitmap = world.print_world();
    render.Renderworld(bitmap);
    std::this_thread::sleep_for(500ms);  
    /*while (render.Renderworld(bitmap))
    {
        world.step(1);
        bitmap = world.print_world();
        std::this_thread::sleep_for(10ms);
    }*/
}

