#include "CAWorld.h"
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include "Model.h"

CAWorld::CAWorld(const Model &model) :
width(std::get<0>(model.world_param)),
height(std::get<1>(model.world_param)),
grid_size(std::get<2>(model.world_param)),
grid(std::vector<std::vector<Cell>>(std::get<1>(model.world_param), std::vector<Cell>(std::get<0>(model.world_param))))
{
	std::vector<std::pair<type_no, percentage>> accum_dist(model.grid_types.size());
	int temp = 0;
	int index = 0;
	for (const std::pair<type_name, Model::grid_param_type_no_name> &pair : model.grid_types)
	{
		Cell::name_to_int[pair.first] = index;
		Cell::int_to_name[index] = pair.first;
		Cell::type_aux_funcs[index] = std::make_tuple(std::get<1>(pair.second), std::get<2>(pair.second), std::get<3>(pair.second));
		temp += std::get<0>(pair.second);
		accum_dist.push_back(std::make_pair(index++, temp));
	}
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			grid[i][j].set_coord(i, j);
			grid[i][j]._set_type(type_initializer(accum_dist));
		}
	}
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			std::get<2>(Cell::type_aux_funcs.at(grid[i][j]._get_type()))(&grid[i][j]);
		}
	}
	diffX[0] = [](){return -1; }; diffY[0] = [](){return 1; };// top left
	diffX[1] = [](){return 0; }; diffY[1] = [](){return 1; };// top
	diffX[2] = [](){return 1; }; diffY[2] = [](){return 1; }; //top right
	diffX[3] = [](){return -1; }; diffY[3] = [](){return 0; }; //left
	diffX[4] = [](){return 1; }; diffY[4] = [](){return 0; };//right
	diffX[5] = [](){return -1; }; diffY[5] = [](){return -1; }; // bottom left
	diffX[6] = [](){return 0; }; diffY[6] = [](){return -1; }; // bottom
	diffX[7] = [](){return 1; }; diffY[7] = [](){return -1; }; // bottom right
}

type_no CAWorld::type_initializer(std::vector<std::pair<type_no, percentage>> &accum_dist)
{
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(1,100);
    int rand = distribution(generator);
    for (size_t i = 0; i != accum_dist.size(); ++i)
    {
        if (rand <= accum_dist[i].second)
            return accum_dist[i].first;
    }
}

void CAWorld::fill_neighbors(std::vector<Cell *> &neighbors, int x, int y)
{
    for (int i = 0; i != diffX.size(); ++i)
    {
        int neighborX = x + diffX[i]();
        int neighborY = y + diffY[i]();
        if (neighborX < 0 || neighborY < 0 || neighborX >= height || neighborY >= width)
            neighbors[i] = nullptr;
        else
            neighbors[i] = &grid[neighborX][neighborY];
    }
}
void CAWorld::step()
{
    for (int i = 0; i != height; ++i)
    {
        for (int j = 0; j != width; ++j)
        {
            std::get<1>(Cell::type_aux_funcs.at(grid[i][j]._get_type()))(&grid[i][j]);
        }
    }
    std::vector<Cell *> neighbors(diffX.size());
    for (int i = 0; i != height; ++i)
    {
        for (int j = 0; j != width; ++j)
        {
            fill_neighbors(neighbors, i ,j);
            std::get<0>(Cell::type_aux_funcs.at(grid[i][j]._get_type()))(&grid[i][j], neighbors);
        }
    }
}

void CAWorld::print_world()
{
	std::ofstream of("output.txt");
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			if (grid[i][j]["open"])
			{
				of << "0,";
			}
			else
				of << "1,";
		}
		of << ";";
	}
}
