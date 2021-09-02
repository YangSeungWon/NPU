#include <iostream>
#include "matrix.h"
std::ostream &operator<<(std::ostream &os, const uint8_t &a) {
    os << (uint32_t)a;
    return os;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &matrix)
{
    for (uint32_t i = 0; i < matrix.rows; i++)
    {
        for (uint32_t j = 0; j < matrix.columns; j++)
        {
            for (uint32_t t = 0; t < j; t++) os << ' ';
            os << matrix(i, j) << ' ';
        }
        os << "------\n";
    }
    os << "==============\n";
    return os;
}
