#ifndef CELL_H
#define CELL_H

#include <vector>
#include <tuple>
#include <string>
#include <functional>
#include <unordered_map>
#include "Model.h"

class Cell
{
friend class CAWorld;
public:
    Cell(){}
    Cell(int x_coord, int y_coord): x(x_coord), y(y_coord) {}
    void set_coord(int x_coord, int y_coord) { x = x_coord; y = y_coord;}
    void set_type(const std::string &s) { type = s; }
    const std::string &get_type() {return type;}
    int &operator[](const std::string &state) {return states[state];}

static inline int countSurroundingCellsWithValue(const std::vector<Cell *> &neighbors, const std::string &state) // this can't be defined in resource file?
{
	int surrounding = 0;
	for (size_t i = 0; i != neighbors.size(); ++i)
	{
		if (neighbors[i] != nullptr && (*neighbors[i])[state])
			++surrounding;
	}
	return surrounding;
}

private:
    static std::unordered_map<std::string, std::tuple<Model::process_type, Model::reset_type, Model::init_type>> type_aux_funcs;
    int x = 0, y = 0;
    std::string type;
    std::unordered_map<std::string, int> states;
};

#endif
