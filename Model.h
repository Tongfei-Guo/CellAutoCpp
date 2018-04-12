#ifndef MODEL_H
#define MODEL_H

#include <tuple>
#include <string>
#include <functional>
#include <unordered_map>
class Cell;
class Model
{
    friend class CAWorld;
	friend class Cell;
public:
    typedef std::tuple<int, int, int> world_param_type; //width, height, grid_size
    typedef std::function<void(Cell *)> reset_type;
    typedef std::function<void(Cell *)> init_type;
    typedef std::function<void(Cell *, std::vector<Cell*> neighbors)> process_type;
    typedef int percentage;
    typedef std::tuple<std::string, percentage, process_type, reset_type , init_type> grid_param_type;
    Model(){}
	Model(world_param_type param) : world_param(param) {}
	Model(world_param_type param, std::vector<grid_param_type> types);
	void add_grid_type(grid_param_type type);
	void add_grid_type(const std::string &name, percentage percent, process_type process, reset_type reset, init_type init);
	void remove_grid_type(const std::string &name); // not implemented.
	void get_grid_type(const std::string &name, const std::string &field_name);// not implemented.

private:
	typedef std::tuple<percentage, process_type, reset_type, init_type> grid_param_type_no_name;
    world_param_type world_param;
    std::unordered_map<std::string, grid_param_type_no_name> grid_types;//use map to easily avoid type of same name.
	void _add_grid_type(grid_param_type type);
};
#endif
