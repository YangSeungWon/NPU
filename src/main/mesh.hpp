#include <iostream>
#include "mesh.h"
template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
std::ostream &operator<<(std::ostream &os, const Mesh<INPUT_T, OUTPUT_T, ACC_T> &mesh)
{
    os << mesh.tile_table;
    return os;
}
