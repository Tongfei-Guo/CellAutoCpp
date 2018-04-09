#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <tuple>
#include <string>
#include <functional>
class Cell;
class Model
{
    friend class CAWorld;
public:
    typedef std::tuple<int, int, int> world_param_type; //width, height, grid_size
    typedef std::function<void(Cell *)> reset_type;
    typedef std::function<void(Cell *)> init_type;
    typedef std::function<void(Cell *, std::vector<Cell*> neighbors)> process_type;
    typedef int percentage;
    typedef std::tuple<std::string, percentage, process_type, reset_type , init_type> grid_param_type;
    Model(){}
	Model(world_param_type param) : world_param(param) {}
    Model(world_param_type param, std::vector<grid_param_type> types) : world_param(param), grid_types(types) {}
	void add_grid_type(const std::string &, percentage, process_type, reset_type, init_type);
private:
    world_param_type world_param;
    std::vector<grid_param_type> grid_types;
};
#endif
