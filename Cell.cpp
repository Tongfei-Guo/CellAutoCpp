#include "Cell.h"
#include <vector>
#include <string>
#include "CATypes.h"

std::unordered_map<type_name, std::tuple<process_type, reset_type, init_type>> Cell::type_aux_funcs;

int Cell::countSurroundingCellsWithValue(const std::vector<Cell *> &neighbors, const state_name &state)
{
	int surrounding = 0;
	for (size_t i = 0; i != neighbors.size(); ++i)
	{

		if (neighbors[i] != nullptr && (*neighbors[i])[state])
			++surrounding;
	}
	return surrounding;
}
