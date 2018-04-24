#include <fstream>
#include <vector>
#include <string>
#include <random>
#include "CAWorld.h"
#include "Model.h"
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cctype>

std::vector<std::function<int()>> CAWorld::diffX = std::vector<std::function<int()>>(8),
                                  CAWorld::diffY = std::vector<std::function<int()>>(8);

CAWorld::CAWorld(const Model &model) :
height(std::get<0>(model.world_param)),
width(std::get<1>(model.world_param)),
grid_size(std::get<2>(model.world_param)),
getcolor(model.getcolor)
{
    // initialize grid
    grid = grid_type(height, std::vector<Cell*>(width));
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
                cells[i*width+j].timestamp_resize(model.buffersize-1);
                grid[i][j] = &cells[i*width+j];
            }
        }
	}
	/*for (auto &row : grid)
	    for (Cell *cell : row)
	        std::cout << cell << std::endl;*/
	//initilize cell tpyes, states ,etc in the grid.
	std::vector<std::pair<type_name, percentage>> accum_dist(model.grid_types.size());
	percentage percent_sum = 0;
	for (const std::pair<type_name, Model::grid_param_type_no_name> &pair : model.grid_types)
	{
		type_name type = Cell::_add_type(pair);
		percentage percent = std::get<0>(pair.second);
		if (percent > 100) // error-check, percentage is unsigned, don't check for < 0
		    throw percentage_error("Model : grid_type : " + type + " has a percentage value of " + std::to_string(percent));
		percent_sum += percent;
		accum_dist.push_back(std::make_pair(type, percent_sum));
	}
	if (percent_sum > 100)
		    throw percentage_error("Model : the sum of all grid_types' percentage is " + std::to_string(percent_sum));
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
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

CAWorld &CAWorld::forall_step(unsigned steps)
{
	for (unsigned i = 0; i != steps; ++i)
	{
		_forall_step();
	}
	return (*this);
}

CAWorld &CAWorld::step(unsigned x, unsigned y)
{
	if ((x > height-1) || (y > width-1))
	_step(x, y);
	return (*this);
}

std::vector<int> CAWorld::print_world()
{
	std::vector<int> bitindex;
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
            if(getcolor != nullptr)
            {
            	int colorind = getcolor(grid[i][j]);  //std::get<3>(  )(&grid[i][j]);
            	bitindex.push_back(colorind);
            }
            else
            {
            	std::cout<<"Warning: the getcolor is not defined"<<std::endl;
            }

		}
	}

	return std::move(bitindex);
}

void CAWorld::print_test(std::vector<frame_type> &frames, unsigned k)
{
	std::ofstream of("output.txt");
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
		    if (frames[k][i][j].type == "")
		        of << "2,";
			else if ((frames[k][i][j])["open"])
			{
				of << "0,";
			}
			else
				of << "1,";
		}
		of << ";";
	}
}


void CAWorld::save2file(char const * filename)
{
	//savefile format :
	//typename , statename1 statename2 statename1 ..., val1 val2 val3 ... \n

	std::fstream savefile;
    savefile.open(filename,std::ios::out);
    for(int i = 0; i < height; i++)
    {
    	for(int j = 0; j != width; j++)
    	{
            Cell * cell = grid[i][j];
            savefile << cell->get_type()<<" , ";
            auto statemap = cell->get_states();
            for(const auto state:statemap) savefile<<state.first<<" ";
            savefile<<",";
            for(const auto state:statemap) savefile<<state.second<<" ";
            savefile<<"\n";
    	}

    }
    savefile.close();

}

void CAWorld::loadfromfile(char const * filename)
{
	//savefile format :
	//typename , statename1 statename2 statename1 ..., val1 val2 val3 ... \n

    std::fstream savefile;
    savefile.open(filename,std::ios::in);
    std::string alineoflog;

	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{

		std::getline(savefile,alineoflog);
		if(savefile.rdstate()!= std::ios::goodbit)
		{
			std::cout<<"something wrong when load the file, please check if the grid is fit into the log"<<std::endl;
		}

			std::stringstream logstream(alineoflog); //get the type name
			std::string statename;
			std::getline(logstream,statename,',');

			statename.erase(std::remove_if(statename.begin(), statename.end(), [](char c){return std::isspace(c);}), statename.end());

			grid[i][j]->set_type(statename);


			std::string namelist;
			std::getline(logstream,namelist,',');
			std::stringstream nametmp(namelist);
			std::istream_iterator<std::string> listit(nametmp);

			std::string vallist;
			std::getline(logstream, vallist, ',');
			std::stringstream valtmp(vallist);
			std::istream_iterator<int> valit(valtmp);


			while(valit != std::istream_iterator<int>())
			{
				auto statename = *(listit++);
				auto stateval  = *(valit++);
				(*grid[i][j])[statename] = stateval;
				std::cout<<statename<<std::endl;
				std::cout <<stateval <<std::endl;
			}

			std::cout<<"\n";
		}
	}
    savefile.close();
}

CAWorld &CAWorld::combine(const CAWorld &world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high)
{
	combine_error_check(world, r_low, r_high, c_low, c_high);
	auto frames_before = grid[r_low][c_low]->timestamp_size();
	for (auto i = r_low; i <= r_high; ++i)
	{
		for (auto j = c_low; j <= c_high; ++j)
		{
		    Cell *curr = world.grid[i - r_low][j - c_low]->_clone();
		    if (dynamic_cast<CellHistBounded*>(curr) != nullptr)
		        grid[i][j]->_move(dynamic_cast<CellHistBounded*>(curr));
			else if (dynamic_cast<CellHistUnbounded*>(curr) != nullptr)
			    grid[i][j]->_move(dynamic_cast<CellHistUnbounded*>(curr));
            else
                grid[i][j]->_move(curr);
		}
	}
	auto frames_after = grid[r_low][c_low]->timestamp_size();
	if (frames_after > frames_before) // this only happens in CellUnboundedCell case.
    {
        for (auto i = 0; i != height; ++i)
        {
            for (auto j = 0; j != width; ++j)
            {
                grid[i][j]->timestamp_resize(frames_after);
            }
        }
    }
	return (*this);
}

CAWorld &CAWorld::combine(CAWorld &&world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high)
{
	combine_error_check(world, r_low, r_high, c_low, c_high);
	auto frames_before = grid[r_low][c_low]->timestamp_size();
	for (auto i = r_low; i <= r_high; ++i)
	{
		for (auto j = c_low; j <= c_high; ++j)
		{
		    Cell *curr = std::move(world.grid[i - r_low][j - c_low])->_clone();
		    if (dynamic_cast<CellHistBounded*>(curr) != nullptr)
		        grid[i][j]->_move(dynamic_cast<CellHistBounded*>(curr));
			else if (dynamic_cast<CellHistUnbounded*>(curr) != nullptr)
			    grid[i][j]->_move(dynamic_cast<CellHistUnbounded*>(curr));
            else
                grid[i][j]->_move(curr);
		}
	}
	auto frames_after = grid[r_low][c_low]->timestamp_size();
	if (frames_after > frames_before) // this only happens in CellUnboundedCell case.
    {
        for (auto i = 0; i <= height; ++i)
        {
            for (auto j = 0; j <= width; ++j)
            {
                grid[i][j]->timestamp_resize(frames_after);
            }
        }
    }
	return (*this);
}

std::vector<frame_type> CAWorld::get_timestamps()
{
    unsigned size = grid[0][0]->timestamp_size();
    std::vector<frame_type> timestamps(size+1, frame_type(height, std::vector<Cell>(width, Cell())));
    for (int j = 0; j != height; ++j)
    {
        for (int k = 0; k != width; ++k)
        {
            timestamps[0][j][k] = grid[j][k]->get_frame(0);
            for (int i = 1; i <= size; ++i)
            {
                timestamps[i][j][k] = grid[j][k]->get_frame(i);
            }
        }
    }
    return timestamps;
}

frame_type CAWorld::get_timestamp()
{
    frame_type timestamp(height, std::vector<Cell>(width, Cell()));
    for (int j = 0; j != height; ++j)
    {
        for (int k = 0; k != width; ++k)
        {
            timestamp[j][k] = grid[j][k]->get_frame(0);
        }
    }
    return timestamp;
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

void CAWorld::_forall_step()
{
	for (auto &row : grid)
	{
		for (Cell *cell : row)
		{
			cell->_call_reset()(cell);
			cell->prepare_process();
		}
	}
	for (auto i = 0; i != height; ++i)
	{
		for (auto j = 0; j != width; ++j)
		{
		    Cell *cell = grid[i][j];
			cell->_call_process()(grid, cell);
		}
	}
}

void CAWorld::_step(unsigned i, unsigned j)
{
    for (auto &row : grid)
	{
		for (Cell *cell : row)
		{
			cell->_call_reset()(cell);
			cell->prepare_process();
		}
	}
	Cell *cell = grid[i][j];
    cell->_call_process()(grid, cell);
}

void CAWorld::copy_grid(const CAWorld &rhs)
{
    grid = grid_type(height, std::vector<Cell*>(width));
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
    else if (dynamic_cast<CellHistBounded*>(rhs.grid[0][0]) != nullptr) //grid cell type is CellHistBounded
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
