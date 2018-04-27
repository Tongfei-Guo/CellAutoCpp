//there are classic model
#include "classicmodel.h"
#include <random>
#include <chrono>
#include <iostream>


CAWorld cyclicmodel(unsigned width, unsigned height, unsigned timestampsize, bool visualizationswitch)
{
    auto process = process_type([] (const grid_type &grid, Cell *self)
    {
        std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
        int next=((*self)["state"]+1)%16;
        bool changing=false;
        for(auto n: neighbors)
            if (n!=NULL)
                changing=changing||((*n)["oldState"]==next);
        if (changing)
            (*self)["state"]=next;
    });
    auto reset = reset_type([](Cell *self)
    {
        (*self)["oldState"]=(*self)["state"];
    });
    auto init = init_type([](Cell *self)
    {
        (*self)["state"] = rand()%16;
    });
    getcolor_type getcolor = nullptr;
    if(visualizationswitch)
    {
		getcolor = getcolor_type([](Cell *self)
		{
			return (*self)["state"];
		});
    }

    	CAWorld world(Model(world_param_type(width,height, 6), { grid_param_type("Cyclic", 100, process, reset, init) },timestampsize, getcolor));


	return world;
}


CAWorld gameoflifemodel(unsigned width, unsigned height,unsigned timestampsize, bool visualizationswitch)
{
    auto process = process_type([] (const grid_type &grid, Cell *self)
    {
        auto neighbors = get_neighbors(grid, self->x, self->y);
        auto cnt=countSurroundingCellsWithValue(neighbors, "wasAlive");
        (*self)["alive"]=(cnt==3 || (cnt==2 && (*self)["alive"]));
    });
    auto reset = reset_type([](Cell *self)
    {
        (*self)["wasAlive"]=(*self)["alive"];
    });
    auto init = init_type([](Cell *self)
    {
        (*self)["alive"] = (rand()>RAND_MAX/2);
    });
    getcolor_type getcolor = nullptr;
    if(visualizationswitch)
    {
		getcolor = getcolor_type([](Cell *self)
		{
			if ((*self)["alive"]==0)
				return 1;
			else
				return 0;
		});
    }
/*
    std::vector<bitcolor> palette = {
    {68, 36, 52, 255},
    {255, 255, 255, 255}
    };
    */

    CAWorld world(Model(world_param_type(width, height, 6), { grid_param_type("living", 100, process, reset, init) },timestampsize, getcolor));


	return world;
}


CAWorld isingmodel(unsigned width, unsigned height,ising_param param,unsigned timestampsize, bool visualizationswitch)
{


	float beta = param.beta;
	float J = param.J;
    float mu = param.mu;
    float H = param.H;


	auto process = process_type([beta,J,mu,H](const grid_type &grid, Cell *self)
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
	getcolor_type getcolor = nullptr;
	if(visualizationswitch)
	{
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
	}

	Model model(world_param_type(width,height, 6), { grid_param_type("electron", 100, process, reset, init) },timestampsize,getcolor);
	CAWorld world(model);

	return world;

}



#define LAVAV 100000000 //1e8

int lavamodel(unsigned width, unsigned height, unsigned timestampsize, bool visualizationswitch)
{

    std::vector<int> colors(64);
	int index = 0;
	for (; index < 18; ++index) { colors[index] = 1; }
	for (; index < 22; ++index) { colors[index] = 0; }
	for (; index < 25; ++index) { colors[index] = 1; }
	for (; index < 27; ++index) { colors[index] = 2; }
	for (; index < 29; ++index) { colors[index] = 3; }
	for (; index < 32; ++index) { colors[index] = 2; }
	for (; index < 35; ++index) { colors[index] = 0; }
	for (; index < 36; ++index) { colors[index] = 2; }
	for (; index < 38; ++index) { colors[index] = 4; }
	for (; index < 42; ++index) { colors[index] = 5; }
	for (; index < 44; ++index) { colors[index] = 4; }
	for (; index < 46; ++index) { colors[index] = 2; }
	for (; index < 56; ++index) { colors[index] = 1; }
	for (; index < 64; ++index) { colors[index] = 0; }

    int W= width;//144*2;
    int H= height; //96*2;
    double pi=acos(-1.0);

    auto process = process_type([] (const grid_type &grid, Cell *self)
    {
        std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
        if ((*self)["droplet"]){
            for(auto c: neighbors)
                if (c!=NULL){
                    (*c)["value"]=(*self)["value"]/2;
                    (*c)["prev"]=(*self)["prev"]/2;
                }
            (*self)["droplet"]=0;
            return;
        }
        int avg=getSurroundingCellsAverageValue(neighbors, "value");
        (*self)["next"]=0.998*(2*avg-(*self)["prev"]);
    });
    auto reset = reset_type([](Cell *self){
        if (rand()>0.99993*RAND_MAX){
            (*self)["value"]=(-0.25+0.3*(rand()*1.0/RAND_MAX))*LAVAV;
            (*self)["prev"]=(*self)["value"];
            (*self)["droplet"]=1;
        }else{
            (*self)["prev"]=(*self)["value"];
            (*self)["value"]=(*self)["next"];
        }
        (*self)["value"]=fmin((int)(0.5*LAVAV), fmax((int)(-0.5*LAVAV), (*self)["value"]));
    });
    auto init = init_type([](Cell *self)
    {
        (*self)["value"] = 0;
        (*self)["prev"] = 0;
        (*self)["next"] = 0;
        (*self)["droplet"] = 0;
    });
    getcolor_type getcolor = nullptr;
    if(visualizationswitch)
    {
		auto getcolor = getcolor_type([pi,W,H,&colors](Cell *self)
		{
			double ret=(*self)["value"]*1.0/LAVAV+0.5+sin(pi*self->x/W)*0.04+sin(pi*self->y/H)*0.04-0.05;
			ret=fmin(0.9999, fmax(0.0, ret));
			return colors[(int)(ret*colors.size())];
		});
    }

/*

    std::vector<bitcolor> palette = {
    {34,10,21,255}, {68,17,26,255}, {123,16,16,255},
    {190,45,16,255}, {244,102,20,255}, {254,212,97,255}
    };

*/

    CAWorld world(Model(world_param_type(W, H, 6), { grid_param_type("lava", 100, process, reset, init) },timestampsize,getcolor));


	return 0;
}







