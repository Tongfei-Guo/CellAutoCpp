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
    static std::unordered_map<type_name, std::tuple<process_type, reset_type, init_type, getcolor_type>> type_aux_funcs;
	std::unordered_map<state_name, state_value> states; // TODO : state can be arbitrary type?
    int x = 0, y = 0;
    type_name type; // save space, can be mapped to its proper type name by type_name.
	static inline const type_name &_add_type(const std::pair<type_name, Model::grid_param_type_no_name> &pair);
	inline void _set_type(const type_name &rhs_type);
	inline const process_type&_call_process() const;
	inline const reset_type&_call_reset() const;
	inline const init_type&_call_init() const;
	inline const getcolor_type &_call_getcolor() const;
	virtual void prepare_process() {} // does nothing in the base class
	// the following functions are for deep copy&move between Cell type and its derived types.
	inline Cell* _clone() const &;
	inline Cell* _clone() && ;
	inline void _move(Cell *cell);
	inline void _move(CellHistBounded *cell);
	inline void _move(CellHistUnbounded *cell);
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
	std::deque<type_name> type_hist;
	std::deque<std::unordered_map<state_name, state_value>> states_hist;
	inline void buffer_resize(unsigned size);
	virtual inline void prepare_process() final;
	virtual inline CellHistBounded *_clone() const & final;
	virtual inline CellHistBounded *_clone() && final;
	virtual inline void _move(Cell *cell) final;
	virtual inline void _move(CellHistBounded *cell) final;
	virtual inline void _move(CellHistUnbounded *cell) final;
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
	std::vector<type_name> type_hist;
	std::vector<std::unordered_map<state_name, state_value>> states_hist;
	virtual inline void prepare_process() final;
	virtual inline CellHistUnbounded *_clone() const & final;
	virtual inline CellHistUnbounded *_clone() && final;
	virtual inline void _move(Cell *cell) final;
	virtual inline void _move(CellHistBounded *cell) final;
	virtual inline void _move(CellHistUnbounded *cell) final;
};

/* Cell inlines */
inline state_value &Cell::operator[](const state_name &state)
{
	return states[state];
}

inline void Cell::set_coord(int x_coord, int y_coord)
{
    x = x_coord; y = y_coord;
}

inline void Cell::set_type(const type_name &rhs_type)
{
	if (type_aux_funcs.find(rhs_type) == type_aux_funcs.end())
    {
        throw nonexist_type("void Cell::set_type(const std::string &) funciton call : type " + rhs_type + " does not exist.");
	}
	type = rhs_type;
}

inline const type_name &Cell::get_type() const
{
	return type;
}

inline const type_name &Cell::_add_type(const std::pair<type_name, Model::grid_param_type_no_name> &pair)
{
	type_aux_funcs[pair.first] = std::make_tuple(std::get<1>(pair.second), std::get<2>(pair.second), std::get<3>(pair.second),std::get<4>(pair.second));
	return pair.first;
}

inline void Cell::_set_type(const type_name &rhs_type)
{
    type = rhs_type;
}

inline const process_type&Cell::_call_process() const
{
	return std::get<0>(type_aux_funcs.at(type));
}

inline const reset_type&Cell::_call_reset() const
{
	return std::get<1>(type_aux_funcs.at(type));
}

inline const init_type&Cell::_call_init() const
{
	return std::get<2>(type_aux_funcs.at(type));
}

inline const getcolor_type&Cell::_call_getcolor() const
{
	return std::get<3>(type_aux_funcs.at(type));
}

inline Cell* Cell::_clone() const &
{
	return new Cell(*this);
}

inline Cell* Cell::_clone() &&
{
	return new Cell(std::move(*this)); // do i need move here?
}

inline void Cell::_move(Cell *cell)
{
	this->type = std::move(cell->type);
	this->states = std::move(cell->states);
}

inline void Cell::_move(CellHistBounded *cell)
{
	this->type = std::move(cell->type);
	this->states = std::move(cell->states);
}
inline void Cell::_move(CellHistUnbounded *cell)
{
	this->type = std::move(cell->type);
	this->states = std::move(cell->states);
}
/* CellHistBounded inlines */
inline void CellHistBounded::buffer_resize(unsigned size)
{
    type_hist.resize(size);
    states_hist.resize(size);
}

inline void CellHistBounded::prepare_process()
{
	type_hist.pop_front();
	type_hist.push_back(type);
	states_hist.pop_front();
	states_hist.push_back(states);
}

inline CellHistBounded *CellHistBounded::_clone() const &
{
	return new CellHistBounded (*this);
}

inline CellHistBounded *CellHistBounded::_clone() &&
{
	return new CellHistBounded (std::move(*this));
}

inline void CellHistBounded::_move(Cell *cell)
{
	this->type = std::move(cell->type);
	this->states = std::move(cell->states);
}

inline void CellHistBounded::_move(CellHistBounded *cell)
{
	(*this) = std::move(*cell);
}

inline void CellHistBounded::_move(CellHistUnbounded *cell)
{
    this->type = std::move(cell->type);
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

inline CellHistUnbounded *CellHistUnbounded::_clone() const &
{
	return new CellHistUnbounded (*this);
}

inline CellHistUnbounded *CellHistUnbounded::_clone() &&
{
	return new CellHistUnbounded (std::move(*this));
}

inline void CellHistUnbounded::_move(Cell *cell)
{
	this->type = std::move(cell->type);
	this->states = std::move(cell->states);
}

inline void CellHistUnbounded::_move(CellHistBounded *cell)
{
    this->type = std::move(cell->type);
	this->states = std::move(cell->states);
	auto size1 = this->type_hist.size(), size2 = cell->type_hist.size();
	this->type_hist = {};
	this->states_hist = {};
	copy(cell->type_hist.cbegin(), cell->type_hist.cend(), std::back_inserter(this->type_hist));
	copy(cell->states_hist.cbegin(), cell->states_hist.cend(), std::back_inserter(this->states_hist));
}

inline void CellHistUnbounded::_move(CellHistUnbounded *cell)
{
	(*this) = std::move(*cell);
}

#endif
