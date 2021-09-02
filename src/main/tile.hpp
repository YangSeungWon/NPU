#include <iostream>
#include "tile.h"

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
Tile<INPUT_T, OUTPUT_T, ACC_T>::Tile(uint32_t _rows, uint32_t _columns): 
    rows(_rows), 
    columns(_columns), 
    processing_element_table(Matrix<ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>>(_rows, _columns))
{
    in_a = new INPUT_T[_rows]();
    in_b = new OUTPUT_T[_columns]();
    in_d = new OUTPUT_T[_columns]();
    in_ctrl = new control_t[_columns]();

    out_a = new INPUT_T[_rows]();
    out_b = new OUTPUT_T[_columns]();
    out_c = new OUTPUT_T[_columns]();
    out_ctrl = new control_t[_columns]();
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
Tile<INPUT_T, OUTPUT_T, ACC_T>::~Tile()
{
    delete[] in_a;
    delete[] in_b;
    delete[] in_d;
    delete[] in_ctrl;
    delete[] out_a;
    delete[] out_b;
    delete[] out_c;
    delete[] out_ctrl;
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void Tile<INPUT_T, OUTPUT_T, ACC_T>::calculate()
{
    for (uint32_t i=0; i<rows; i++)
    {
        processing_element_table(i, 0).input_a(in_a[i]);
    }
    
    for (uint32_t j=0; j<columns; j++)
    {
        processing_element_table(0, j).input_b(in_b[j]); 
        processing_element_table(0, j).input_d(in_d[j]); 
        processing_element_table(0, j).input_ctrl(in_ctrl[j]); 
    }

    for (int32_t i = rows-1; i >= 0; i--)
    {
        for (int32_t j = columns-1; j >= 0; j--)
        {
            if (j > 0)
            {
                processing_element_table(i, j).input_a(processing_element_table(i, j-1).out_a);
            }
            if (i > 0)
            {
                processing_element_table(i, j).input_b(processing_element_table(i-1, j).out_b);
                processing_element_table(i, j).input_d(processing_element_table(i-1, j).out_c);
                processing_element_table(i, j).input_ctrl(processing_element_table(i-1, j).out_ctrl);
            }
            processing_element_table(i, j).calculate();
        }
    }

    for (uint32_t i=0; i<rows; i++)
    {
        out_a[i] = processing_element_table(i, columns-1).out_a;
    }
    for (uint32_t j=0; j<columns; j++)
    {
        out_b[j] = processing_element_table(rows-1, j).out_b;
        out_c[j] = processing_element_table(rows-1, j).out_c;
        out_ctrl[j] = processing_element_table(rows-1, j).out_ctrl;
    }
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void Tile<INPUT_T, OUTPUT_T, ACC_T>::input_a(INPUT_T* data)
{
    for (uint32_t i=0; i<rows; i++)
       in_a[i] = data[i];
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void Tile<INPUT_T, OUTPUT_T, ACC_T>::input_b(INPUT_T *data)
{
    for (uint32_t j=0; j<columns; j++)
        in_b[j] = (OUTPUT_T)data[j];
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void Tile<INPUT_T, OUTPUT_T, ACC_T>::input_b(OUTPUT_T *data)
{
    for (uint32_t j=0; j<columns; j++)
        in_b[j] = data[j];
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void Tile<INPUT_T, OUTPUT_T, ACC_T>::input_d(INPUT_T *data)
{
    for (uint32_t j=0; j<columns; j++)
        in_d[j] = (OUTPUT_T)data[j];
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void Tile<INPUT_T, OUTPUT_T, ACC_T>::input_d(OUTPUT_T *data)
{
    for (uint32_t j=0; j<columns; j++)
        in_d[j] = data[j];
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void Tile<INPUT_T, OUTPUT_T, ACC_T>::input_ctrl(control_t *data)
{
    for (uint32_t j=0; j<columns; j++)
        in_ctrl[j] = data[j];
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
std::ostream &operator<<(std::ostream &os, const Tile<INPUT_T, OUTPUT_T, ACC_T> &tile)
{
    os << tile.processing_element_table;
    return os;
}
