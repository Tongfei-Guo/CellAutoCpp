#include "CAFunctions.h"
#include "CAWorld.h"
#include "Cell.h"
#include <vector>
#include <utility>

int countSurroundingCellsWithValue(const std::vector<Cell *> &neighbors, const state_name &state)
{
	int surrounding = 0;
	for (size_t i = 0; i != neighbors.size(); ++i)
	{

		if (neighbors[i] != nullptr && (*neighbors[i])[state])
			++surrounding;
	}
	return surrounding;
}

std::vector<Cell*> get_neighbors(const grid_type &grid, int x, int y)
{
    std::vector<Cell*> neighbors(CAWorld::diffX.size());
	for (auto i = 0; i != CAWorld::diffX.size(); ++i)
	{
		int neighborX = x + CAWorld::diffX[i]();
		int neighborY = y + CAWorld::diffY[i]();
		if (neighborX < 0 || neighborY < 0 || neighborX >= grid.size() || neighborY >= grid[0].size())
			neighbors[i] = nullptr;
		else
			neighbors[i] = grid[neighborX][neighborY];
	}
	return neighbors;
}

std::pair<unsigned, unsigned> get_coord(const grid_type &grid, Cell *cell)
{
    if (grid.size() == 0)
        throw get_coord_error("grid has 0 dimension");
    else if ((grid.back().back() < cell) || (grid.front().front() > cell))
        throw get_coord_error("cell is not within grid");
    else
    {
        unsigned long cell_size = (unsigned long long)grid[0][1] - (unsigned long long)grid[0][0];// we cannot just take sizeof(Cell) due to polymorphism.
        auto diff = ((unsigned long long)cell - (unsigned long long)grid.front().front())/cell_size;
        unsigned x = 0, y = 0, width = grid[0].size();
        x = diff / width;
        y = diff % width;
        return std::make_pair(x, y);
    }
}
