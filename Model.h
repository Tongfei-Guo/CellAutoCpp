#ifndef MODEL_H
#define MODEL_H

#include "CATypes.h"
#include <tuple>
#include <string>
#include <functional>
#include <unordered_map>

struct bitcolor
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char alpha;
} ;


class Model
{
    friend class CAWorld;
	friend class Cell;
public:
    Model(){}
	Model(world_param_type param, std::vector<bitcolor> colorpalette , std::vector<grid_param_type> types = {}, unsigned size = 1); // TODO : error-check user input percent is within 0 - 100, and all percentage sum up to 100.

	void add_grid_type(grid_param_type type);
	void add_grid_type(const type_name &name, percentage percent, process_type process, reset_type reset, init_type init, getcolor_type getcolor);
	void remove_grid_type(const type_name &name); // TODO : not implemented.
	void get_grid_type(const type_name &name, const std::string &field_name);// TODO : not implemented.

private:
	typedef std::tuple<percentage, process_type, reset_type, init_type, getcolor_type> grid_param_type_no_name;
    world_param_type world_param;
	std::unordered_map<type_name, grid_param_type_no_name> grid_types;//use map to easily avoid type of same name.
	unsigned buffersize; // the number of timestamps we want to keep, 0 meaning keep all timestamps.
	inline void _add_grid_type(grid_param_type &type);
	std::vector<bitcolor> palette;
};


inline void Model::_add_grid_type(grid_param_type &type)
{
	type_name name = std::move(std::get<0>(type));
	grid_param_type_no_name param = grid_param_type_no_name(std::move(std::get<1>(type)), std::move(std::get<2>(type)),
	                                                        std::move(std::get<3>(type)), std::move(std::get<4>(type)),
															std::move(std::get<5>(type)));
	grid_types[std::move(name)] = std::move(param);

}

#endif
