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
	inline state_value &operator[](const state_name &state);
	void set_coord(int x_coord, int y_coord);
	inline void set_type(const type_name &rhs_type);
	inline const type_name &get_type() const;
	static int countSurroundingCellsWithValue(const std::vector<Cell *> &neighbors, const state_name &state);

private:
    static std::unordered_map<type_no, std::tuple<process_type, reset_type, init_type>> type_aux_funcs;
	static std::unordered_map<type_name, type_no> type_name_to_int;
	static std::unordered_map<type_no, type_name> type_int_to_name;// mimic a bimap.
	static std::unordered_map<state_name, state_no> state_name_to_int;
	static std::unordered_map<state_no, state_name> state_int_to_name;
	std::unordered_map<state_no, state_value> states; // TODO : state can be arbitrary type?
    int x = 0, y = 0;
    type_no type; // save space, can be mapped to its proper type name by type_name.
	inline void _set_type(const type_no &rhs_type);
	inline const type_no &_get_type() const; 
	inline const process_type&_call_process() const;
	inline const reset_type&_call_reset() const;
	inline const init_type&_call_init() const;
};

inline state_value &Cell::operator[](const state_name &state) 
{
	static state_no counter = 0;
	try
	{
		state_name_to_int.at(state);
	}
	catch (std::out_of_range &) // populate state_name_to_int and state_name_to_int at run-time when init called.
	{
		state_name_to_int[state] = counter;
		state_int_to_name[counter++] = state;
	}
	return states[state_name_to_int[state]]; 
}

inline void Cell::set_coord(int x_coord, int y_coord) 
{
    x = x_coord; y = y_coord; 
}

inline void Cell::set_type(const type_name &rhs_type)
{
	try
	{
		int rhs_type_no = type_name_to_int.at(rhs_type);
		_set_type(rhs_type_no);
	}
	catch (std::out_of_range &)//at out of range
	{
		throw nonexist_type("void Cell::set_type(const std::string &) funciton call : type " + rhs_type + " does not exist.");
	}
}

inline const type_name &Cell::get_type() const
{
	return type_int_to_name.at(type);
}

inline void Cell::_set_type(const type_no &rhs_type)
{
	type = rhs_type;
}

inline const type_no &Cell::_get_type() const 
{
	return type;
}

inline const process_type&Cell::_call_process() const
{
	return std::get<0>(type_aux_funcs[type]);
}

inline const reset_type&Cell::_call_reset() const
{
	return std::get<1>(type_aux_funcs[type]);
}

inline const init_type&Cell::_call_init() const
{
	return std::get<2>(type_aux_funcs[type]);
}
#endif
