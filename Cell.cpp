#include "Cell.h"
#include <vector>
#include <string>
#include "CATypes.h"

std::unordered_map<type_no, std::tuple<process_type, reset_type, init_type,getcolor_type >> Cell::type_aux_funcs;

std::unordered_map<type_name, type_no> Cell::type_name_to_int;

std::unordered_map<type_no, type_name> Cell::type_int_to_name;

std::unordered_map<state_name, state_no> Cell::state_name_to_int;

std::unordered_map<state_no, state_name> Cell::state_int_to_name;

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
