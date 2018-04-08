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
    typedef std::vector<std::tuple<std::string, percentage, process_type, reset_type , init_type>> grid_param_type;
    Model(){}
    Model(world_param_type param, grid_param_type types) : world_param(param), grid_types(types) {}
private:
    world_param_type world_param;
    grid_param_type grid_types;
};
#endif
