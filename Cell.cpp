#include "Cell.h"
#include <vector>
#include <string>
#include "CATypes.h"

std::unordered_map<type_no, std::tuple<process_type, reset_type, init_type>> Cell::type_aux_funcs;

std::unordered_map<type_name, type_no> Cell::name_to_int;

std::unordered_map<type_no, type_name> Cell::int_to_name;

int Cell::countSurroundingCellsWithValue(const std::vector<Cell *> &neighbors, const std::string &state)
{
	int surrounding = 0;
	for (size_t i = 0; i != neighbors.size(); ++i)
	{
		if (neighbors[i] != nullptr && (*neighbors[i])[state])
			++surrounding;
	}
	return surrounding;
}