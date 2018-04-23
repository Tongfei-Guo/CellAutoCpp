#include <fstream>
#include <vector>
#include <string>
#include <random>
#include "CAWorld.h"
#include "Model.h"

std::vector<std::function<int()>> CAWorld::diffX = std::vector<std::function<int()>>(8),
                                  CAWorld::diffY = std::vector<std::function<int()>>(8);

CAWorld::CAWorld(const Model &model) :
width(std::get<0>(model.world_param)),
height(std::get<1>(model.world_param)),
grid_size(std::get<2>(model.world_param))
{
    // initialize grid
    grid = std::vector<std::vector<Cell*>>(height, std::vector<Cell*>(width));
	if (model.buffersize == 1)
	{
	    Cell *cells = new Cell[height*width];
	    for (auto i = 0; i != height; ++i)
        {
            for (auto j = 0; j != width; ++j)
            {
                grid[i][j] = &cells[i*width+j];
            }
        }
	}
	else if (model.buffersize == 0)
	{
	    CellHistUnbounded *cells = new CellHistUnbounded[height*width];
	    for (auto i = 0; i != height; ++i)
        {
            for (auto j = 0; j != width; ++j)
            {
                grid[i][j] = &cells[i*width+j];
            }
        }
	}
	else
	{
	    CellHistBounded *cells = new CellHistBounded[height*width];
	    for (auto i = 0; i != height; ++i)
        {
            for (auto j = 0; j != width; ++j)
            {
                cells[i*width+j].buffer_resize(model.buffersize);
                grid[i][j] = &cells[i*width+j];
            }
        }
	}
	//initilize cell tpyes, states ,etc in the grid.
	std::vector<std::pair<type_name, percentage>> accum_dist(model.grid_types.size());
	unsigned temp = 0;
	for (const std::pair<type_name, Model::grid_param_type_no_name> &pair : model.grid_types)
	{
		type_name type = Cell::_add_type(pair);
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
		for (Cell *cell : row)
		{
			cell->_call_init()(cell);
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

CAWorld::CAWorld(const CAWorld &rhs): height(rhs.height), width(rhs.width), grid_size(rhs.grid_size)
{
    copy_grid(rhs);
}

CAWorld::CAWorld(CAWorld &&rhs) noexcept : height(rhs.height), width(rhs.width), grid_size(rhs.grid_size)
{
    rhs.height = rhs.width = 0;
    grid = std::move(rhs.grid);
}

CAWorld &CAWorld::operator=(const CAWorld &rhs)
{
    if (this == &rhs) return *this;
    delete_grid();
    height = rhs.height;
    width = rhs.width;
    grid_size = rhs.grid_size;
    copy_grid(rhs);
}

CAWorld &CAWorld::operator=(CAWorld &&rhs) noexcept
{
    // this will take care of the destruction of the moved-to object's original grid.
    std::swap(height, rhs.height);
    std::swap(width, rhs.width);
    std::swap(grid_size, rhs.grid_size);
    std::swap(grid, rhs.grid);
}

CAWorld::~CAWorld()
{
    delete_grid();
}

void CAWorld::step(unsigned steps)
{
	for (unsigned i = 0; i != steps; ++i)
	{
		_step();
	}
}

std::vector<int> CAWorld::print_world()
{
	std::vector<int> bitindex;
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			 int colorind = grid[i][j]->_call_getcolor()(grid[i][j]);  //std::get<3>(  )(&grid[i][j]);
			 bitindex.push_back(colorind);

		}
	}

	return std::move(bitindex);
}

void CAWorld::print_test()
{
	std::ofstream of("output.txt");
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			if ((*grid[i][j])["open"])
			{
				of << "0,";
			}
			else
				of << "1,";
		}
		of << ";";
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

type_name CAWorld::type_initializer(const std::vector<std::pair<type_name, percentage>> &accum_dist)
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
			neighbors[i] = grid[neighborX][neighborY];
	}
}

void CAWorld::_step()
{
	for (auto &row : grid)
	{
		for (Cell *cell : row)
		{
			cell->_call_reset()(cell);
		}
	}
	std::vector<Cell *> neighbors(diffX.size());
	for (auto i = 0; i != height; ++i)
	{
		for (auto j = 0; j != width; ++j)
		{
		    Cell *cell = grid[i][j];
			cell->prepare_process();
			fill_neighbors(neighbors, i, j);
			cell->_call_process()(cell, neighbors);
		}
	}
}

void CAWorld::copy_grid(const CAWorld &rhs)
{
    grid = std::vector<std::vector<Cell*>>(height, std::vector<Cell*>(width));
    if (dynamic_cast<CellHistUnbounded*>(rhs.grid[0][0]) != nullptr) //grid cell type is CellHistUnbounded
    {
        CellHistUnbounded *cells = new CellHistUnbounded[height*width];
        CellHistUnbounded *rhs_cells = dynamic_cast<CellHistUnbounded*>(rhs.grid[0][0]);
	    for (auto i = 0; i != height; ++i)
        {
            for (auto j = 0; j != width; ++j)
            {
                cells[i*width+j] = rhs_cells[i*width+j];
                grid[i][j] = &cells[i*width+j];
            }
        }
    }
    if (dynamic_cast<CellHistBounded*>(rhs.grid[0][0]) != nullptr) //grid cell type is CellHistBounded
    {
        CellHistBounded *cells = new CellHistBounded[height*width];
        CellHistBounded *rhs_cells = dynamic_cast<CellHistBounded*>(rhs.grid[0][0]);
	    for (auto i = 0; i != height; ++i)
        {
            for (auto j = 0; j != width; ++j)
            {
                cells[i*width+j] = rhs_cells[i*width+j];
                grid[i][j] = &cells[i*width+j];
            }
        }
    }
    else//grid cell type is Cell
    {
        Cell *cells = new Cell[height*width];
        Cell *rhs_cells = rhs.grid[0][0];
	    for (auto i = 0; i != height; ++i)
        {
            for (auto j = 0; j != width; ++j)
            {
                cells[i*width+j] = rhs_cells[i*width+j];
                grid[i][j] = &cells[i*width+j];
            }
        }
    }
}

void CAWorld::delete_grid()
{
    if (height != 0 && width != 0)
    {
        if (dynamic_cast<CellHistUnbounded*>(grid[0][0]) != nullptr) //grid cell type is CellHistUnbounded
        {
            CellHistUnbounded *lhs_cells = dynamic_cast<CellHistUnbounded*>(grid[0][0]);
            delete [] lhs_cells;
        }
        else if (dynamic_cast<CellHistBounded*>(grid[0][0]) != nullptr) //grid cell type is CellHistUnbounded
        {
            CellHistBounded *lhs_cells = dynamic_cast<CellHistBounded*>(grid[0][0]);
            delete [] lhs_cells;
        }
        else //grid cell type is Cell
        {
            Cell *lhs_cells = grid[0][0];
            delete [] lhs_cells;
        }
    }
}
