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

struct bitcolor
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char alpha;
} ;

#define V 100000000 //1e8

int main()
{

    std::vector<bitcolor> palette = {};
    std::vector<int> colors;

    for(int i=0; i<64; i++){
        palette.push_back(bitcolor{(unsigned char)89, (unsigned char)125, (unsigned char)206, (unsigned char)(255/64*i)});
        colors.push_back(63-i);
    }

    int W=96*2;
    int H=64*2;
    double pi=acos(-1.0);

    auto process = process_type([] (const grid_type &grid, Cell *self)
    {
        std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
        if ((*self)["droplet"]){
            for(auto &c: neighbors)
                if (c!=NULL){
                    (*c)["value"]=(*self)["value"]/2;
                    (*c)["prev"]=(*self)["prev"]/2;
                }
            (*self)["droplet"]=0;
            return;
        }
        int avg=getSurroundingCellsAverageValue(neighbors, "value");
        (*self)["next"]=0.99*(2*avg-(*self)["prev"]);
    });
    auto reset = reset_type([](Cell *self){
        if (rand()>0.9999*RAND_MAX){
            (*self)["value"]=(-0.2+0.25*(rand()*1.0/RAND_MAX))*V;
            (*self)["prev"]=(*self)["value"];
            (*self)["droplet"]=1;
        }else{
            (*self)["prev"]=(*self)["value"];
            (*self)["value"]=(*self)["next"];
        }
    });
    auto init = init_type([](Cell *self)
    {
        (*self)["value"] = 0;
        (*self)["prev"] = 0;
        (*self)["next"] = 0;
        (*self)["droplet"] = 0;
    });
    auto getcolor = getcolor_type([pi,W,H,&colors](Cell *self)
    {
        //var v = (Math.max(2 * this.value + 0.02, 0) - 0.02) + 0.5;
			//return colors[Math.floor(colors.length * v)];

        double ret=fmax(2.0*(*self)["value"]/V+0.02,0)+0.48;
        return colors[(int)(ret*colors.size())];
    });

    CAWorld world(Model(world_param_type(W, H, 6), { grid_param_type("water", 100, process, reset, init) },1));
	//auto start = std::chrono::high_resolution_clock::now();
	world.forall_step(1000);
    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
    //auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    //std::cout << nanoseconds << "nanoseconds\n";
    world.AddMeasureAndRun(new CADistributionMeasure("Final Dist"));

    for(auto m: world.GetMeasures())
        std::cout<<"========="<<m->GetName()<<"========="<<std::endl<<m->Str_All()<<std::endl;

	return 0;
}
