#include "Model.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

void Model::_add_grid_type(grid_param_type type)
{
	std::string name = std::get<0>(type);
	grid_param_type_no_name param = grid_param_type_no_name(std::get<1>(type), std::get<2>(type), std::get<3>(type), std::get<4>(type)); 
	grid_types[std::move(name)] = std::move(param);
}

//should we warn user when grid type of the same name is being added?
Model::Model(world_param_type param, std::vector<grid_param_type> types) : world_param(param) 
{
	for (const auto &type : types)
	{
		_add_grid_type(type);
	}
}

void Model::add_grid_type(grid_param_type type)
{
	_add_grid_type(type);
}

void Model::add_grid_type(const std::string &name, percentage percent, process_type process, reset_type reset, init_type init)
{
	grid_param_type_no_name param = grid_param_type_no_name(percent, process, reset, init);
	grid_types[name] = std::move(param);
}