#include <iostream>
#include "matrix.h"

template <typename T>
Matrix<T>::Matrix(uint32_t _rows, uint32_t _columns): 
    rows(_rows), 
    columns(_columns)
{
    matrix = new T **[_rows];
    for (uint32_t i = 0; i < _rows; i++)
    {
        matrix[i] = new T *[_columns];
        for (uint32_t j = 0; j < _columns; j++)
            matrix[i][j] = new T();
    }
}

template <typename T>
Matrix<T>::Matrix(uint32_t _rows, uint32_t _columns, uint32_t _arg1, uint32_t _arg2):
    rows(_rows),
    columns(_columns)
{
    matrix = new T **[_rows];
    for (uint32_t i = 0; i < _rows; i++)
    {
        matrix[i] = new T *[_columns];
        for (uint32_t j = 0; j < _columns; j++)
            matrix[i][j] = new T(_arg1, _arg2);
    }
}

template <typename T>
void Matrix<T>::fill(T* mat)
{
    for (uint32_t i = 0; i < rows; i++)
        for (uint32_t j = 0; j < columns; j++)
            *matrix[i][j] = mat[i*columns+j];
}

template <typename T>
Matrix<T>::~Matrix()
{
    // std::cout << "free a matrix " << matrix << '\n';
    for (uint32_t i = 0; i < rows; i++)
    {
        for (uint32_t j = 0; j < columns; j++)
        {
            delete matrix[i][j];
        }
        delete matrix[i];
    }
    delete matrix;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T>& mat)
{
    for (uint32_t i = 0; i < mat.rows; i++)
        for (uint32_t j = 0; j < mat.columns; j++)
            *matrix[i][j] = mat(i, j);
    return *this;
}

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
