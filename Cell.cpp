#include "Cell.h"
#include <vector>
#include <string>
#include "CATypes.h"

std::map<type_name, std::tuple<process_type, reset_type, init_type>> Cell::type_aux_funcs;
