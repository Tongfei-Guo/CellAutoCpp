#include "Model.h"
#include <vector>
#include <string>

void Model::add_grid_type(const std::string &type, percentage percent, process_type process, reset_type reset, init_type init)
{
	grid_types.push_back(std::make_tuple(type, percent, process, reset, init));
}

