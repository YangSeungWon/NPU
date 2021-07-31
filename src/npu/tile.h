#ifndef TILE_H_
#define TILE_H_

#include <cstdint>
#include "processing_element.h"

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
class Tile
{
public:
    explicit Tile(Stationary _stationary, uint32_t _latency, uint32_t _rows, uint32_t _columns)
        : stationary(_stationary), latency(_latency), rows(_rows), columns(_columns) {}
    void calculate();
    static Stationary stationary;
    static uint32_t latency;
    ProcessingElement** processing_element_table;

private:
    INPUT_T  *in_a;
    OUTPUT_T *in_b;
    OUTPUT_T *in_d;
    INPUT_T  *out_a;
    OUTPUT_T *out_b;
    OUTPUT_T *out_c;
    
    uint32_t rows;
    uint32_t columns;
};

#endif
