#ifndef CELL_H
#define CELL_H

#include <vector>
#include <tuple>
#include <string>
#include <functional>
#include <unordered_map>
#include <deque>
#include <algorithm>
#include <memory>
#include "CATypes.h"
#include "CAException.h"
#include "Model.h"
class CellHistBounded; //forward dependency
class CellHistUnbounded; //forward dependency
class Cell
{
friend class CAWorld;
friend class CellHistBounded;
friend class CellHistUnbounded;
public:
	// Cell copy-control
    Cell(){}
    Cell(int x_coord, int y_coord): x(x_coord), y(y_coord) {}
	Cell(const Cell &) = default;
	Cell(Cell &&) noexcept = default;
	Cell& operator=(const Cell &) = default;
	Cell& operator=(Cell &&) noexcept = default;
	virtual ~Cell() = default;
	// auxiliary functions
	inline state_value &operator[](const state_name &state);
	void set_coord(int x_coord, int y_coord);
	inline void set_type(const type_name &rhs_type);
	inline const type_name &get_type() const;
	static int countSurroundingCellsWithValue(const std::vector<Cell *> &neighbors, const state_name &state);

protected:
    static std::unordered_map<type_no, std::tuple<process_type, reset_type, init_type, getcolor_type>> type_aux_funcs;
	static std::unordered_map<type_name, type_no> type_name_to_int;
	static std::unordered_map<type_no, type_name> type_int_to_name;// mimic a bimap.
	static std::unordered_map<state_name, state_no> state_name_to_int;
	static std::unordered_map<state_no, state_name> state_int_to_name;
	std::unordered_map<state_no, state_value> states; // TODO : state can be arbitrary type?
    int x = 0, y = 0;
    type_no type; // save space, can be mapped to its proper type name by type_name.
	static inline type_no _add_type(const std::pair<type_name, Model::grid_param_type_no_name> &pair);
	inline void _set_type(const type_no &rhs_type);
	inline const type_no &_get_type() const;
	inline const process_type&_call_process() const;
	inline const reset_type&_call_reset() const;
	inline const init_type&_call_init() const;
	inline const getcolor_type &_call_getcolor() const;
	virtual void prepare_process() {} // does nothing in the base class
	// the following functions are for deep copy&move between Cell type and its derived types.
	inline std::unique_ptr<Cell> _clone() const &;
	inline std::unique_ptr<Cell> _clone() && ;
	inline void _move(const std::unique_ptr<Cell> &cell);
	inline void _move(const std::unique_ptr<CellHistBounded> &cell);
	inline void _move(const std::unique_ptr<CellHistUnbounded> &cell);
};

class CellHistBounded : public Cell
{
friend class CellHistUnbounded;
public:
	CellHistBounded(unsigned buffersize) : Cell(), type_hist(buffersize), states_hist(buffersize) {}
	CellHistBounded(const CellHistBounded &) = default;
	CellHistBounded(CellHistBounded &&) noexcept = default;
	CellHistBounded& operator=(const CellHistBounded &) = default;
	CellHistBounded& operator=(CellHistBounded &&) noexcept = default;
	virtual ~CellHistBounded() final = default;
private:
	std::deque<type_no> type_hist;
	std::deque<std::unordered_map<state_no, state_value>> states_hist;
	virtual inline void prepare_process() final;
	virtual inline std::unique_ptr<CellHistBounded> _clone() const & final;
	virtual inline std::unique_ptr<CellHistBounded> _clone() && final;
	virtual inline void _move(const std::unique_ptr<Cell> &cell) final;
	virtual inline void _move(const std::unique_ptr<CellHistBounded> &cell) final;
	virtual inline void _move(const std::unique_ptr<CellHistUnbounded> &cell) final;
};

class CellHistUnbounded : public Cell
{
friend class CellHistBounded;
public:
	CellHistUnbounded() = default;
	CellHistUnbounded(const CellHistUnbounded &) = default;
	CellHistUnbounded(CellHistUnbounded &&) noexcept = default;
	CellHistUnbounded& operator=(const CellHistUnbounded &) = default;
	CellHistUnbounded& operator=(CellHistUnbounded &&) noexcept = default;
	virtual ~CellHistUnbounded() final = default;
private:
	std::vector<type_no> type_hist;
	std::vector<std::unordered_map<state_no, state_value>> states_hist;
	virtual inline void prepare_process() final;
	virtual inline std::unique_ptr<CellHistUnbounded> _clone() const & final;
	virtual inline std::unique_ptr<CellHistUnbounded> _clone() && final;
	virtual inline void _move(const std::unique_ptr<Cell> &cell) final;
	virtual inline void _move(const std::unique_ptr<CellHistBounded> &cell) final;
	virtual inline void _move(const std::unique_ptr<CellHistUnbounded> &cell) final;
};

/* Cell inlines */
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

inline type_no Cell::_add_type(const std::pair<type_name, Model::grid_param_type_no_name> &pair)
{
	static type_no counter = 0;
	type_name_to_int[pair.first] = counter;
	type_int_to_name[counter] = pair.first;
	type_aux_funcs[counter] = std::make_tuple(std::get<1>(pair.second), std::get<2>(pair.second), std::get<3>(pair.second),std::get<4>(pair.second));
	return counter++;
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

inline const getcolor_type&Cell::_call_getcolor() const
{
	return std::get<3>(type_aux_funcs[type]);
}

inline std::unique_ptr<Cell> Cell::_clone() const &
{
	return std::make_unique<Cell>(*this);
}

inline std::unique_ptr<Cell> Cell::_clone() &&
{
	return std::make_unique<Cell>(std::move(*this)); // do i need move here?
}

inline void Cell::_move(const std::unique_ptr<Cell> &cell)
{
	this->type = cell->type;
	this->states = std::move(cell->states);
}

inline void Cell::_move(const std::unique_ptr<CellHistBounded> &cell)
{
	this->type = cell->type;
	this->states = std::move(cell->states);
}
inline void Cell::_move(const std::unique_ptr<CellHistUnbounded> &cell)
{
	this->type = cell->type;
	this->states = std::move(cell->states);
}
/* CellHistBounded inlines */
inline void CellHistBounded::prepare_process()
{
	type_hist.pop_front();
	type_hist.push_back(type);
	states_hist.pop_front();
	states_hist.push_back(states);
}

inline std::unique_ptr<CellHistBounded> CellHistBounded::_clone() const &
{
	return std::make_unique<CellHistBounded>(*this);
}

inline std::unique_ptr<CellHistBounded> CellHistBounded::_clone() &&
{
	return std::make_unique<CellHistBounded>(std::move(*this));
}

inline void CellHistBounded::_move(const std::unique_ptr<Cell> &cell)
{
	this->type = cell->type;
	this->states = std::move(cell->states);
}

inline void CellHistBounded::_move(const std::unique_ptr<CellHistBounded> &cell)
{
	(*this) = std::move(*cell);
}

inline void CellHistBounded::_move(const std::unique_ptr<CellHistUnbounded> &cell)
{
    this->type = cell->type;
	this->states = std::move(cell->states);
	auto size1 = this->type_hist.size(), size2 = cell->type_hist.size();
	copy(cell->type_hist.crbegin(), cell->type_hist.crbegin() - std::min(size1, size2), this->type_hist.rbegin());
	copy(cell->states_hist.crbegin(), cell->states_hist.crbegin() - std::min(size1, size2), this->states_hist.rbegin());
}

/*CellHistUnbounded inlines*/
void CellHistUnbounded::prepare_process()
{
	type_hist.push_back(type);
	states_hist.push_back(states);
}

inline std::unique_ptr<CellHistUnbounded> CellHistUnbounded::_clone() const &
{
	return std::make_unique<CellHistUnbounded>(*this);
}

inline std::unique_ptr<CellHistUnbounded> CellHistUnbounded::_clone() &&
{
	return std::make_unique<CellHistUnbounded>(std::move(*this));
}

inline void CellHistUnbounded::_move(const std::unique_ptr<Cell> &cell)
{
	this->type = cell->type;
	this->states = std::move(cell->states);
}

inline void CellHistUnbounded::_move(const std::unique_ptr<CellHistBounded> &cell)
{
    this->type = cell->type;
	this->states = std::move(cell->states);
	auto size1 = this->type_hist.size(), size2 = cell->type_hist.size();
	this->type_hist = {};
	this->states_hist = {};
	copy(cell->type_hist.cbegin(), cell->type_hist.cend(), std::back_inserter(this->type_hist));
	copy(cell->states_hist.cbegin(), cell->states_hist.cend(), std::back_inserter(this->states_hist));
}

inline void CellHistUnbounded::_move(const std::unique_ptr<CellHistUnbounded> &cell)
{
	(*this) = std::move(*cell);
}

#endif
