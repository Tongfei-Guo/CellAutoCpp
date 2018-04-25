#ifndef CA_FUNCTIONS_H
#define CA_FUNCTIONS_H

#include "CATypes.h"
#include "CAException.h"
#include <vector>
#include <utility>
class Cell;
int countSurroundingCellsWithValue(const std::vector<Cell *> &neighbors, const state_name &state);//count number of neighboring cells has states value greater than or equal to 1

std::vector<Cell*> get_neighbors(const grid_type &grid, int x, int y);//find the neighbors of the cell given by grid[x][y]

std::pair<unsigned, unsigned> get_coord(const grid_type &grid, Cell *cell);//get coordinates of cell from grid

#endif // CA_FUNCTIONS_H
