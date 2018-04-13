#include "Cell.h"
#include "CAException.h"
#include <vector>
#include <string>
#include "CATypes.h"

std::unordered_map<type_no, std::tuple<process_type, reset_type, init_type>> Cell::type_aux_funcs;

std::unordered_map<type_name, type_no> Cell::name_to_int;

std::unordered_map<type_no, type_name> Cell::int_to_name;

void Cell::set_type(const type_name &rhs_type)
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

const type_name &Cell::get_type() const
{
	return int_to_name.at(type);
}
