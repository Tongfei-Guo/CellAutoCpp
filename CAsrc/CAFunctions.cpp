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

int getSurroundingCellsAverageValue(const std::vector<Cell *> &neighbors, const state_name &state)
{
	int sum = 0, num = 0;
	for (size_t i = 0; i != neighbors.size(); ++i)
	{

		if (neighbors[i] != nullptr){
			num++;
			sum+=(*neighbors[i])[state];
		}

	}
	return sum/num;
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
