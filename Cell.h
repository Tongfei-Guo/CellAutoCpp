#ifndef CELL_H
#define CELL_H

#include <vector>
#include <tuple>
#include <string>
#include <functional>
#include <unordered_map>
#include "CATypes.h"
#include "CAException.h"

class Cell
{
friend class CAWorld;
public:
    Cell(){}
    Cell(int x_coord, int y_coord): x(x_coord), y(y_coord) {}
	inline int &operator[](const std::string &state);
	void set_coord(int x_coord, int y_coord);
	inline void set_type(const type_name &rhs_type);
	inline const type_name &get_type() const;
	static int countSurroundingCellsWithValue(const std::vector<Cell *> &neighbors, const std::string &state);

private:
    static std::unordered_map<type_no, std::tuple<process_type, reset_type, init_type>> type_aux_funcs;
	static std::unordered_map<type_name, type_no> name_to_int;
	static std::unordered_map<type_no, type_name> int_to_name;// mimic a bimap.
    int x = 0, y = 0;
    int type; // save space, can be mapped to its proper type name by type_name.
    std::unordered_map<std::string, int> states; // TODO : state can be arbitrary type?
	inline void _set_type(const type_no &rhs_type);
	inline const type_no &_get_type() const; 
};

inline int &Cell::operator[](const std::string &state) { return states[state]; }

inline void Cell::set_coord(int x_coord, int y_coord) { x = x_coord; y = y_coord; }

inline void Cell::set_type(const type_name &rhs_type)
{
	try
	{
		int rhs_type_no = name_to_int.at(rhs_type);
		_set_type(rhs_type_no);
	}
	catch (std::out_of_range &)//at out of range
	{
		throw nonexist_type("void Cell::set_type(const std::string &) funciton call : type " + rhs_type + " does not exist.");
	}
}

inline const type_name &Cell::get_type() const
{
	return int_to_name.at(type);
}

inline void Cell::_set_type(const type_no &rhs_type)
{
	type = rhs_type;
}

inline const type_no &Cell::_get_type() const // this can't be defined in resource file?
{
	return type;
}

#endif
