#ifndef TILE_H_
#define TILE_H_

#include <cstdint>
#include <iostream>
#include "matrix.h"
#include "processing_element.h"

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
class Tile
{
public:
    Tile(uint32_t _rows, uint32_t _columns);
    void calculate();
    void input_a(INPUT_T *data);
    void input_b(INPUT_T *data);
    void input_b(OUTPUT_T *data);
    void input_d(INPUT_T *data);
    void input_d(OUTPUT_T *data);
    void input_ctrl(control_t *data);
    ~Tile();
    template <typename INPUT_T_, typename OUTPUT_T_, typename ACC_T_>
    friend std::ostream &operator<<(std::ostream &os, const Tile<INPUT_T_, OUTPUT_T_, ACC_T_> &tile);

    Matrix<ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>> processing_element_table;

    INPUT_T *in_a;
    OUTPUT_T *in_b;
    OUTPUT_T *in_d;
    control_t *in_ctrl;

    INPUT_T *out_a;
    OUTPUT_T *out_b;
    OUTPUT_T *out_c;
    control_t *out_ctrl;
    
private:
    uint32_t rows;
    uint32_t columns;
};

#include "tile.hpp"
#endif
