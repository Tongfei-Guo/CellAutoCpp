#ifndef CA_TYPES_H
#define CA_TYPES_H

#include <vector>
#include <functional>
class Cell;

typedef std::tuple<int, int, int> world_param_type; //width, height, grid_size
typedef std::string type_name;//TODO : make this template type? seems unnecessary
typedef int type_no;
typedef int percentage;
typedef std::function<void(Cell *)> reset_type;
typedef std::function<void(Cell *)> init_type;
typedef std::function<void(Cell *, std::vector<Cell*> neighbors)> process_type;
typedef std::tuple<type_name, percentage, process_type, reset_type, init_type> grid_param_type;
typedef std::string state_name;//TODO : make this template type? seems unnecessary
typedef int state_value; //TODO : make this template type? potentially value could be int , double, or something else.
typedef int state_no;

#endif
