//Ising model
//Involing process

//init: randomly generate a distribute {x_1,x_2,x_3,...,x_n} \in {-1,1}^n
//step 1: randomly select a site from grid and flip the state value (-1 --> 1, 1 --> -1)
//step 2: compute accpetance = exp(-beta * ( H_new - H_old)) where H = -J * sum(x_i,x_j)
//step 3: flip probability = min{1,accpetance}
//step 4: confirm flip at flip probability
#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include <cmath>
#include <iostream>
void runIsing()
{

	auto process = process_type([](const grid_type &grid, Cell *self)
	{
		std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
		int surrounding = countSurroundingCellsWithValue(neighbors, "wasOpen");


		float beta = 0.5;
		float J = 2;
        float mu = 0.005;
        float H = 1;

		state_value currentori = (*self)["ori"];
		state_value flipedori = (*self)["ori"] * -1;

		std::cout<<"get state"<<std::endl;

		float Hnew = 0, Hold = 0;
		for(auto adjescent : neighbors)
		{
			if(adjescent == nullptr) continue;
            //compute new energy
            Hnew += -J*(*adjescent)["ori"]*flipedori  - mu*H*flipedori;;
			//compute odd energy
            Hold += -J*(*adjescent)["ori"]*currentori  - mu*H*currentori;
		}

		float accpetance = std::exp(-beta * (Hnew - Hold));
		accpetance = accpetance > 1 ? 1 : accpetance;

		if((double) ((double) rand() / (RAND_MAX)) < accpetance )  //do flip
			(*self)["ori"] *= -1;

	});
	auto reset = reset_type([](Cell *self)
	{
		;//empty //(*self)["wasOpen"] = (*self)["open"];
	});
	auto init = init_type([](Cell *self)
	{
		(*self)["ori"] = (((double) rand() / (RAND_MAX)) > 0.5)  ? 1:-1; //  (((double) rand() / (RAND_MAX)) > 0.4);
	});
	auto getcolor = getcolor_type([](Cell *self)
	{
		if(self->get_type() == "electron")
		{
			if((*self)["ori"] == 1)
				return 0;
			else
				return 1;
		}
	});


	Model model(world_param_type(50, 50, 6), { grid_param_type("electron", 100, process, reset, init) },1,getcolor);
	CAWorld world(model);
	world.step(1,1);
	world.print_world();


}
