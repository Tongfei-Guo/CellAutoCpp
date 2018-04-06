#include "CellAuto.h"
#include <fstream>
#include <vector>
#include <string>
std::unordered_map<std::string, std::tuple<Model::process_type, Model::reset_type, Model::init_type>> Cell::type_aux_funcs;
void CAWorld::print_world()
{
    std::ofstream of("output.txt");
    for (int i = 0; i != height; ++i)
    {
        for (int j = 0; j != width; ++j)
        {
            if (grid[i][j]["open"])
            {
                of << "0,";
            }
            else
                of << "1,";
        }
        of << ";";
    }
}
std::string CAWorld::type_initializer(std::vector<std::pair<std::string, int>> &accum_dist)
{
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(1,100);
    int rand = distribution(generator);
    for (size_t i = 0; i != accum_dist.size(); ++i)
    {
        if (rand <= accum_dist[i].second)
            return accum_dist[i].first;
    }
}
CAWorld::CAWorld(const Model &model):width(std::get<0>(model.world_param)),
                                height(std::get<1>(model.world_param)),
                                grid_size(std::get<2>(model.world_param)),
                                grid(std::vector<std::vector<Cell>>(height, std::vector<Cell>(width)))
                                {
                                    std::vector<std::pair<std::string, int>> accum_dist(model.grid_types.size());
                                    int temp = 0;
                                    for (int i = 0; i != model.grid_types.size(); ++i)
                                    {
                                        Cell::type_aux_funcs[std::get<0>(model.grid_types[i])] = make_tuple(std::get<2>(model.grid_types[i]), std::get<3>(model.grid_types[i]), std::get<4>(model.grid_types[i]));
                                        accum_dist[i].first = std::get<0>(model.grid_types[i]);
                                        temp = accum_dist[i].second = temp + std::get<1>(model.grid_types[i]);
                                    }
                                    for (int i = 0; i != height; ++i)
                                    {
                                        for (int j = 0; j != width; ++j)
                                        {
                                            grid[i][j].set_coord(i, j);
                                            grid[i][j].set_type(type_initializer(accum_dist));
                                        }
                                    }
                                    for (int i = 0; i != height; ++i)
                                    {
                                        for (int j = 0; j != width; ++j)
                                        {
                                            std::get<2>(Cell::type_aux_funcs.at(grid[i][j].get_type()))(&grid[i][j]);
                                        }
                                    }
                                    diffX[0] = [](){return -1;}; diffY[0] = [](){return 1;};// top left
                                    diffX[1] = [](){return 0;}; diffY[1] = [](){return 1;};// top
                                    diffX[2] = [](){return 1;}; diffY[2] = [](){return 1;}; //top right
                                    diffX[3] = [](){return -1;}; diffY[3] = [](){return 0;}; //left
                                    diffX[4] = [](){return 1;}; diffY[4] = [](){return 0;};//right
                                    diffX[5] = [](){return -1;}; diffY[5] = [](){return -1;}; // bottom left
                                    diffX[6] = [](){return 0;}; diffY[6] = [](){return -1;}; // bottom
                                    diffX[7] = [](){return 1;}; diffY[7] = [](){return -1;}; // bottom right
                                }
void CAWorld::fill_neighbors(std::vector<Cell *> &neighbors, int x, int y)
{
    for (int i = 0; i != diffX.size(); ++i)
    {
        int neighborX = x + diffX[i]();
        int neighborY = y + diffY[i]();
        if (neighborX < 0 || neighborY < 0 || neighborX >= height || neighborY >= width)
            neighbors[i] = nullptr;
        else
            neighbors[i] = &grid[neighborX][neighborY];
    }
}
void CAWorld::step()
{
    for (int i = 0; i != height; ++i)
    {
        for (int j = 0; j != width; ++j)
        {
            std::get<1>(Cell::type_aux_funcs.at(grid[i][j].get_type()))(&grid[i][j]);
        }
    }
    std::vector<Cell *> neighbors(diffX.size());
    for (int i = 0; i != height; ++i)
    {
        for (int j = 0; j != width; ++j)
        {
            fill_neighbors(neighbors, i ,j);
            std::get<0>(Cell::type_aux_funcs.at(grid[i][j].get_type()))(&grid[i][j], neighbors);
        }
    }
}
