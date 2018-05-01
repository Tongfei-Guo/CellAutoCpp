//test purpose, don't delete until production
#include <random>
#include "Model.h"
#include "Cell.h"
#include "CAWorld.h"
#include "CATypes.h"
#include "CAFunctions.h"
#include <chrono>
#include <iostream>
#include "CAConsoleViz.h"
int main()
{
    int CHANCE_TO_IGNITE = 0.0001*RAND_MAX;
    int CHANCE_TO_GROW = 0.01*RAND_MAX;
    auto process = process_type([CHANCE_TO_IGNITE, CHANCE_TO_GROW] (const grid_type &grid, Cell *self)
    {
        if ((*self)["wasBurning"])
            (*self)["burning"]-=3;
        else if ((*self)["alive"]){
            std::vector<Cell*> neighbors = get_neighbors(grid, self->x, self->y);
            int cnt=countSurroundingCellsWithValue(neighbors, "wasBurning");
            if (cnt || rand()<CHANCE_TO_IGNITE){
                (*self)["burning"]=9;
                (*self)["alive"]=0;
            }
        }else if (rand()<CHANCE_TO_GROW)
            (*self)["alive"]=1;
    });
    auto reset = reset_type([](Cell *self){
        (*self)["wasBurning"]=((*self)["burning"]!=0);
    });
    auto init = init_type([](Cell *self)
    {
        (*self)["burning"] = 0;
    });
    auto getcolor = getcolor_type([](Cell *self)
    {
    	if ((*self)["burning"])
            return (*self)["burning"];
        else
            return (*self)["alive"]?10:11;
    });

/*

    std::vector<bitcolor> palette = {
    {208,70,72, 0}, {208,70,72, 28}, {208,70,72, 56}, {208,70,72, 84},
    {208,70,72, 112}, {208,70,72, 140}, {208,70,72, 168}, {208,70,72, 196},
    {208,70,72, 224}, {208, 70, 72, 255}, {52, 101, 36, 255}, {255, 255, 255, 255}
    };

*/

    CAWorld world(Model(world_param_type(96, 64, 6), { grid_param_type("tree", 100, process, reset, init) },1));

    CAConsoleViz myviz(&world);

    myviz.SetColorScheme(
        [](Cell *cell)->char{
            if ((*cell)["burning"])
                return 'X';
            else
                return (*cell)["alive"]?'.':' ';
        }
    );

    myviz.Visualize(1000,10);

	return 0;
}
