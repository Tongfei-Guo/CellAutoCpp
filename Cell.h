#ifndef CELL_H
#define CELL_H

#include <vector>
#include <tuple>
#include <string>
#include <functional>
#include <unordered_map>
#include "CATypes.h"

class Cell
{
friend class CAWorld;
public:
	typedef std::string type_name;
    Cell(){}
    Cell(int x_coord, int y_coord): x(x_coord), y(y_coord) {}
    void set_coord(int x_coord, int y_coord) { x = x_coord; y = y_coord;}
	inline void set_type(const type_name &rhs_type);
	inline const type_name &get_type() const;
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
    static std::unordered_map<type_no, std::tuple<process_type, reset_type, init_type>> type_aux_funcs;
	static std::unordered_map<type_name, type_no> name_to_int;
	static std::unordered_map<type_no, type_name> int_to_name;// mimic a bimap.
    int x = 0, y = 0;
    int type; // save space, can be mapped to its proper type name by type_name.
    std::unordered_map<std::string, int> states; // TODO : state can be arbitrary type?
	inline void _set_type(const type_no &rhs_type)
	{
		type = rhs_type;
	}
	inline const type_no &_get_type() const // this can't be defined in resource file?
	{
		return type;
	}
};

#endif
