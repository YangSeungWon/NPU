#ifndef MATRIX_H_
#define MATRIX_H_

#include <cstdint>
#include <iostream>
#include <cassert>

template <typename T>
class Matrix
{
public:
    Matrix(uint32_t _rows, uint32_t _columns);
    Matrix(uint32_t _rows, uint32_t _columns, uint32_t _arg1, uint32_t _arg2);
    void fill(T* mat);
    ~Matrix();

    T &get(uint32_t i, uint32_t j) { assert(i>=0 && i<rows); assert(j>=0 && j<columns); return *matrix[i][j]; }
    T &operator()(uint32_t i, uint32_t j) { assert(i>=0 && i<rows); assert(j>=0 && j<columns); return *matrix[i][j]; }
    const T &operator()(uint32_t i, uint32_t j) const { assert(i>=0 && i<rows); assert(j>=0 && j<columns); return *matrix[i][j]; }
    Matrix<T> &operator=(const Matrix<T> &mat);
    template <typename T_>
    friend std::ostream &operator<<(std::ostream &os, const Matrix<T_> &matrix);

private:
    T ***matrix;
    uint32_t rows;
    uint32_t columns;
};

#include "matrix.hpp"
#endif