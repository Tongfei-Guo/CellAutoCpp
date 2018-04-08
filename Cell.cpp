#include "Cell.h"
#include <vector>
#include <string>

std::unordered_map<std::string, std::tuple<Model::process_type, Model::reset_type, Model::init_type>> Cell::type_aux_funcs;

