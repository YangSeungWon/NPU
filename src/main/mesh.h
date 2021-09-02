#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <cstdint>
#include <iostream>
#include "matrix.h"
#include "tile.h"
#include "processing_element.h"

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
class Mesh
{
public:
    class Request
    {
    public:
        Request(bool* _finished, Stationary _stationary, bool _propagate, uint32_t _size, Matrix<INPUT_T>* _in_a, Matrix<INPUT_T>* _in_b, Matrix<INPUT_T>* _in_d, Matrix<OUTPUT_T>* _out_b, Matrix<OUTPUT_T>* _out_c);
        bool* finished;

        uint32_t preload;
        uint32_t progress;
        uint32_t size;
        control_t control;

        Matrix<INPUT_T>* in_a;
        Matrix<INPUT_T>* in_b;
        Matrix<INPUT_T>* in_d;

        Matrix<OUTPUT_T>* out_b;
        Matrix<OUTPUT_T>* out_c;
    };
    Mesh(uint32_t _rows, uint32_t _columns, uint32_t _tile_rows, uint32_t _tile_columns);
    void calculate();
    
    template <typename INPUT_T_, typename OUTPUT_T_, typename ACC_T_>
    friend std::ostream &operator<<(std::ostream &os, const Mesh<INPUT_T_, OUTPUT_T_, ACC_T_> &mesh);

    Matrix<Tile<INPUT_T, OUTPUT_T, ACC_T>> tile_table;
    std::vector<Request> waiting;

private:
    uint32_t clock;

    uint32_t rows;
    uint32_t columns;
    uint32_t tile_rows;
    uint32_t tile_columns;
};

#include "mesh.hpp"
#endif
