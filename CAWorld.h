#ifndef CAWORLD_H
#define CAWORLD_H

#include <vector>
#include <functional>
#include <string>
#include <memory>
#include "CATypes.h"
#include "Cell.h"
#include "CAFunctions.h"
class Model; //forward dependency

class CAWorld
{
friend std::vector<Cell*> get_neighbors(grid_type &grid, int x, int y);
public:
    CAWorld(const Model &model);
    CAWorld(const CAWorld &rhs, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high);
	CAWorld(const CAWorld &rhs);
	CAWorld(CAWorld &&rhs) noexcept;
	CAWorld &operator=(const CAWorld &rhs);
	CAWorld &operator=(CAWorld &&rhs) noexcept;
	~CAWorld();
	CAWorld &step(unsigned x, unsigned y);
	CAWorld &forall_step(unsigned steps);
	std::vector<int> print_world();
    void print_test(std::vector<frame_type> &frames, unsigned k);//don't delete until production.

    void save2file(const char * filename); //save the type and states of Cells in grid into file
    void loadfromfile(const char * filename); //load the type and states into Cells in grid from file

    CAWorld &combine(const CAWorld &world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high);
	CAWorld &combine(CAWorld &&world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high);

	std::vector<frame_type> get_timestamps();
    frame_type get_timestamp();
    static std::vector<std::function<int()>> diffX, diffY;
private:
    unsigned width, height, grid_size;
	grid_type grid;
	bool empty_reset = 1;
	unsigned buffersize;
	void combine_error_check(const CAWorld &world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high);
	const type_name _add_type(std::pair<type_name, Model::grid_param_type_no_name> &pair);
    type_name type_initializer(const std::vector<std::pair<type_name, percentage>> &accum_dist);
	void _forall_step();
	void _step(unsigned x, unsigned y);
	void copy_grid(const CAWorld &rhs);
	void delete_grid();
    getcolor_type getcolor;
};
#endif
