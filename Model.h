#ifndef MODEL_H
#define MODEL_H

#include "CATypes.h"
#include <tuple>
#include <string>
#include <functional>
#include <unordered_map>

class Model
{
    friend class CAWorld;
	friend class Cell;
public:
    Model(){}
	Model(world_param_type param) : world_param(param) {}
	Model(world_param_type param, std::vector<grid_param_type> types); // TODO : error-check user input percent is within 0 - 100, and all percentage sum up to 100.
	void add_grid_type(grid_param_type type);
	void add_grid_type(const type_name &name, percentage percent, process_type process, reset_type reset, init_type init);
	void remove_grid_type(const type_name &name); // TODO : not implemented.
	void get_grid_type(const type_name &name, const std::string &field_name);// TODO : not implemented.

private:
	typedef std::tuple<percentage, process_type, reset_type, init_type> grid_param_type_no_name;
    world_param_type world_param;
	std::unordered_map<type_name, grid_param_type_no_name> grid_types;//use map to easily avoid type of same name.
	inline void _add_grid_type(grid_param_type &type);
};
#endif
