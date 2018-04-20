#include <fstream>
#include <vector>
#include <string>
#include <random>
#include "CAWorld.h"
#include "Model.h"

CAWorld::CAWorld(const Model &model) :
width(std::get<0>(model.world_param)),
height(std::get<1>(model.world_param)),
grid_size(std::get<2>(model.world_param))
{
	// initialize grid
	if (model.buffersize == 1)
	{
	    for (auto i = 0; i != height; ++i)
        {
            std::vector<std::unique_ptr<Cell>> vec;
            for (auto j = 0; j != width; ++j)
            {
                vec.push_back(std::make_unique<Cell>());
            }
            grid.push_back(std::move(vec));
        }
	}
	else if (model.buffersize == 0)
	{
	    for (auto i = 0; i != height; ++i)
        {
            std::vector<std::unique_ptr<Cell>> vec;
            for (auto j = 0; j != width; ++j)
            {
                vec.push_back(std::make_unique<CellHistUnbounded>());
            }
            grid.push_back(std::move(vec));
        }
	}
	else
	{
	    for (auto i = 0; i != height; ++i)
        {
            std::vector<std::unique_ptr<Cell>> vec;
            for (auto j = 0; j != width; ++j)
            {
                vec.push_back(std::make_unique<CellHistBounded>(model.buffersize));
            }
            grid.push_back(std::move(vec));
        }
	}
	//initilize cell tpyes, states ,etc in the grid.
	std::vector<std::pair<type_no, percentage>> accum_dist(model.grid_types.size());
	int temp = 0;
	int index = 0;
	for (const std::pair<type_name, Model::grid_param_type_no_name> &pair : model.grid_types)
	{
		type_no type = Cell::_add_type(pair);   //register functions initialize, process, reset, getcolor
		temp += std::get<0>(pair.second);
		accum_dist.push_back(std::make_pair(type, temp));
	}
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			grid[i][j]->set_coord(i, j);
			grid[i][j]->_set_type(type_initializer(accum_dist));
		}
	}
	for (auto &row : grid)// don't combine this for loop with the one above, because some init protentially needs the type of neighboring grids.
	{
		for (std::unique_ptr<Cell> &cell : row)
		{
			cell->_call_init()(cell.get());
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

    palette = std::move(model.palette);
}

void CAWorld::step(unsigned steps)
{
	for (unsigned i = 0; i != steps; ++i)
	{
		_step();
	}
}

void CAWorld::print_world()
{
	std::ofstream of("output.txt");
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			 int colorind = grid[i][j]->_call_getcolor()(grid[i][j].get());  //std::get<3>(  )(&grid[i][j]);
			 bitcolor colorinrgb = palette[colorind];
			 of <<colorinrgb.R<<" "<<colorinrgb.G<<" "<<colorinrgb.B<<" "<<colorinrgb.alpha<<",";

		}
	}
}

CAWorld &CAWorld::combine(const CAWorld &world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high)
{
	combine_error_check(world, r_low, r_high, c_low, c_high);
	for (auto i = r_low; i <= r_high; ++i)
	{
		for (auto j = c_low; j <= c_high; ++j)
		{
			this->grid[i][j]->_move(world.grid[i - r_low][j - c_low]->_clone());
		}
	}
	return (*this);
}

CAWorld &CAWorld::combine(CAWorld &&world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high)
{
	combine_error_check(world, r_low, r_high, c_low, c_high);
	for (auto i = r_low; i <= r_high; ++i)
	{
		for (auto j = c_low; j <= c_high; ++j)
		{
			this->grid[i][j]->_move(std::move(world.grid[i - r_low][j - c_low])->_clone());
		}
	}
	return (*this);
}

void CAWorld::combine_error_check(const CAWorld &world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high)
{
	if (r_high < r_low)
		throw combine_error("CAWorld &CAWorld::combine(const CAWorld &, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high) funciton call : r_high = " + std::to_string(r_high) + " cannot be less than r_low = " + std::to_string(r_low));
	else if (c_high < c_low)
		throw combine_error("CAWorld &CAWorld::combine(const CAWorld &, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high) funciton call : c_high = " + std::to_string(c_high) + " cannot be less than c_low = " + std::to_string(c_low));
	else if (world.height != r_high - r_low + 1)
		throw combine_error("CAWorld &CAWorld::combine(const CAWorld &, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high) funciton call : input CAWorld height " + std::to_string(world.height) + " does not agree with input lower bound " + std::to_string(r_low) + " and input upper bound " + std::to_string(r_high));
	else if (world.width != c_high - c_low + 1)
		throw combine_error("CAWorld &CAWorld::combine(const CAWorld &, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high) funciton call : input CAWorld width " + std::to_string(world.width) + " does not agree with input lower bound " + std::to_string(c_low) + " and input upper bound " + std::to_string(c_high));
	else if (r_high > this->height - 1)
		throw combine_error("CAWorld &CAWorld::combine(const CAWorld &, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high) funciton call : r_high = " + std::to_string(r_high) + "out of bound: " + std::to_string(this->height - 1));
	else if (r_high > this->width - 1)
		throw combine_error("CAWorld &CAWorld::combine(const CAWorld &, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high) funciton call : c_high = " + std::to_string(c_high) + "out of bound: " + std::to_string(this->width - 1));
}

type_no CAWorld::type_initializer(const std::vector<std::pair<type_no, percentage>> &accum_dist)
{
	static std::default_random_engine generator;
	static std::uniform_int_distribution<int> distribution(1, 100);
	int rand = distribution(generator);
	for (auto i = 0; i != accum_dist.size(); ++i)
	{
		if (rand <= accum_dist[i].second)
			return accum_dist[i].first;
	}

}

void CAWorld::fill_neighbors(std::vector<Cell *> &neighbors, int x, int y)
{
	for (auto i = 0; i != diffX.size(); ++i)
	{
		int neighborX = x + diffX[i]();
		int neighborY = y + diffY[i]();
		if (neighborX < 0 || neighborY < 0 || neighborX >= height || neighborY >= width)
			neighbors[i] = nullptr;
		else
			neighbors[i] = grid[neighborX][neighborY].get();
	}
}

void CAWorld::_step()
{
	for (auto &row : grid)
	{
		for (std::unique_ptr<Cell> &cell : row)
		{
			cell->_call_reset()(cell.get());
		}
	}
	std::vector<Cell *> neighbors(diffX.size());
	for (auto i = 0; i != height; ++i)
	{
		for (auto j = 0; j != width; ++j)
		{
		    std::unique_ptr<Cell> &cell = grid[i][j];
			cell->prepare_process();
			fill_neighbors(neighbors, i, j);
			cell->_call_process()(cell.get(), neighbors);
		}
	}
}
