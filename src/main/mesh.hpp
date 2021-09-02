#include <iostream>
#include <cassert>
#include "mesh.h"

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
Mesh<INPUT_T, OUTPUT_T, ACC_T>::Request::Request(bool* _finished, Stationary _stationary, bool _propagate, uint32_t _size, Matrix<INPUT_T>* _in_a, Matrix<INPUT_T>* _in_b, Matrix<INPUT_T>* _in_d, Matrix<OUTPUT_T>* _out_b, Matrix<OUTPUT_T>* _out_c): 
    finished(_finished),
    preload(_stationary == Stationary::weight ? 0 : _size), 
    progress(0), 
    control({_stationary, _propagate}), 
    size(_size), 
    in_a(_in_a), 
    in_b(_in_b), 
    in_d(_in_d), 
    out_b(_out_b), 
    out_c(_out_c) {}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
Mesh<INPUT_T, OUTPUT_T, ACC_T>::Mesh(uint32_t _rows, uint32_t _columns, uint32_t _tile_rows, uint32_t _tile_columns): 
    clock(0),
    rows(_rows), 
    columns(_columns), 
    tile_rows(_tile_rows), 
    tile_columns(_tile_columns),
    tile_table(Matrix<Tile<INPUT_T, OUTPUT_T, ACC_T>>(_rows, _columns, _tile_rows, _tile_columns)) {}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void Mesh<INPUT_T, OUTPUT_T, ACC_T>::calculate()
{
    assert(waiting.size() > 0);
    auto *cur = &waiting[0];

}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
std::ostream &operator<<(std::ostream &os, const Mesh<INPUT_T, OUTPUT_T, ACC_T> &mesh)
{
    os << mesh.tile_table;
    return os;
}
