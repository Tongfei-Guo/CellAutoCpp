#ifndef CAWORLD_H
#define CAWORLD_H

#include <vector>
#include <functional>
#include <string>
#include <memory>
#include "CATypes.h"
#include "Cell.h"
class Model; //forward dependency
struct bitcolor;

class CAWorld
{
public:
    CAWorld(const Model &model);
	CAWorld(const std::string &model_name);//no implemented, load built-in model
	CAWorld(const CAWorld &) = default;
	CAWorld(CAWorld &&) noexcept = default;
	CAWorld &operator=(const CAWorld &) = default;
	CAWorld &operator=(CAWorld &&) noexcept = default;
	~CAWorld() = default; // TODO : free cell memory.
	void step(unsigned steps);
    void print_world();
    void print_test();//don't delete until production.
	CAWorld &combine(const CAWorld &world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high);
	CAWorld &combine(CAWorld &&world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high);

private:
    int width = 0, height = 0, grid_size = 0;
	std::vector<std::vector<Cell*>> grid;
    std::vector<std::function<int()>> diffX = std::vector<std::function<int()>>(8), diffY = std::vector<std::function<int()>>(8);
	void combine_error_check(const CAWorld &world, unsigned r_low, unsigned r_high, unsigned c_low, unsigned c_high);
    type_name type_initializer(const std::vector<std::pair<type_name, percentage>> &accum_dist);
    void fill_neighbors(std::vector<Cell*> &neighbors, int x, int y);
	void _step();
    std::vector<bitcolor> palette;
};
#endif
