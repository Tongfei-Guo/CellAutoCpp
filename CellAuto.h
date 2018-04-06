#ifndef CELL_AUTO_H
#define CELL_AUTO_H
#endif

#include <vector>
#include <tuple>
#include <string>
#include <functional>
#include <unordered_map>
#include <random>

class Cell;
class CAWorld;
class Model
{
    friend class CAWorld;
public:
    typedef std::tuple<int, int, int> world_param_type; //width, height, grid_size
    typedef std::function<void(Cell *)> reset_type;
    typedef std::function<void(Cell *)> init_type;
    typedef std::function<void(Cell *, std::vector<Cell*> neighbors)> process_type;
    typedef int percentage;
    typedef std::vector<std::tuple<std::string, percentage, process_type, reset_type , init_type>> grid_param_type;
    Model(world_param_type param, grid_param_type types) : world_param(param), grid_types(types) {}
private:
    world_param_type world_param;
    grid_param_type grid_types;
};

class Cell
{
friend CAWorld;
public:
    Cell(){}
    Cell(int x_coord, int y_coord): x(x_coord), y(y_coord) {}
    void set_coord(int x_coord, int y_coord) { x = x_coord; y = y_coord;}
    void set_type(const std::string &s) { type = s; }
    const std::string &get_type() {return type;}
    int &operator[](const std::string &state) {return states[state];}
    static inline int countSurroundingCellsWithValue(const std::vector<Cell *> &neighbors, const std::string &state)
    {
        int surrounding = 0;
        for (size_t i = 0; i != neighbors.size(); ++i)
        {
            if (neighbors[i] != nullptr && (*neighbors[i])[state])
                ++surrounding;
        }
        return surrounding;
    }
private:
    static std::unordered_map<std::string, std::tuple<Model::process_type, Model::reset_type, Model::init_type>> type_aux_funcs;
    int x = 0, y = 0;
    std::string type;
    std::unordered_map<std::string, int> states;
};

class CAWorld
{
public:
    CAWorld(const Model &model);
    void step();
    void print_world();
private:
    int width = 0, height = 0, grid_size = 0;
    std::vector<std::vector<Cell>> grid;
    std::vector<std::function<int()>> diffX = std::vector<std::function<int()>>(8), diffY = std::vector<std::function<int()>>(8);
    std::string type_initializer(std::vector<std::pair<std::string, int>> &accum_dist);
    void fill_neighbors(std::vector<Cell *> &neighbors, int x, int y);
};
