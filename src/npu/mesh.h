#ifndef MESH_H_
#define MESH_H_

#include <cstdint>
#include "tile.h"
#include "processing_element.h"

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
class Mesh
{
public:
    explicit Mesh(Stationary _stationary, uint32_t _latency, uint32_t _rows, uint32_t _columns, uint32_t _tile_rows, uint32_t _tile_columns)
        : stationary(_stationary), latency(_latency), rows(_rows), columns(_columns), tile_rows(_tile_rows), tile_columns(_tile_columns) {}
    void calculate();
    static Stationary stationary;
    static uint32_t latency;
    Tile** tile_table;

private:
    INPUT_T  **in_a;
    OUTPUT_T **in_b;
    OUTPUT_T **in_d;
    OUTPUT_T **out_b;
    OUTPUT_T **out_c;
    
    uint32_t rows;
    uint32_t columns;
    uint32_t tile_rows;
    uint32_t tile_columns;
};

#endif
