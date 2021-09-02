#include <iostream>
#include "tile.h"
template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
std::ostream &operator<<(std::ostream &os, const Tile<INPUT_T, OUTPUT_T, ACC_T> &tile)
{
    os << tile.processing_element_table;
    return os;
}
