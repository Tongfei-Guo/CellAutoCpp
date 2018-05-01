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
#include "CAConsoleViz.h"

#define V 100000000 //1e8

int main()
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

    int W=99;
    int H=66;
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
            (*self)["value"]=(-0.25+0.3*(rand()*1.0/RAND_MAX))*V;
            (*self)["prev"]=(*self)["value"];
            (*self)["droplet"]=1;
        }else{
            (*self)["prev"]=(*self)["value"];
            (*self)["value"]=(*self)["next"];
        }
        (*self)["value"]=fmin((int)(0.5*V), fmax((int)(-0.5*V), (*self)["value"]));
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
/*
var v = this.value + 0.5
    + Math.sin(this.x / world.width * Math.PI) * 0.04
    + Math.sin(this.y / world.height * Math.PI) * 0.04
    - 0.05;
v = Math.min(1.0, Math.max(0.0, v));

return colors[Math.floor(colors.length * v)];
*/
        double ret=(*self)["value"]*1.0/V+0.5+sin(pi*self->x/W)*0.04+sin(pi*self->y/H)*0.04-0.05;
        ret=fmin(0.9999, fmax(0.0, ret));
        return colors[(int)(ret*colors.size())];
    });

/*

    std::vector<bitcolor> palette = {
    {34,10,21,255}, {68,17,26,255}, {123,16,16,255},
    {190,45,16,255}, {244,102,20,255}, {254,212,97,255}
    };

*/


    CAWorld world(Model(world_param_type(W, H, 6), { grid_param_type("lava", 100, process, reset, init) },1));

    CAConsoleViz myviz(&world);

    myviz.SetColorScheme(
        [pi,W,H](Cell *cell)->char{

          //std::cout<<"water level"<<(*self)["levl"]<<std::endl;
          double ret=(*cell)["value"]*1.0/V+0.5+sin(pi*cell->x/W)*0.04+sin(pi*cell->y/H)*0.04-0.05;
          ret=fmin(0.9999, fmax(0.0, ret));
          int iret=ret*10;

          if (iret<=2)
            return '.';
          else if (iret<=4)
            return '-';
          else if (iret<=8)
            return '+';
          else
            return 'O';
        }
    );

    myviz.Visualize(1000,10);

	return 0;
}
