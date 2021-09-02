#include <iostream>
#include "processing_element.h"

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
std::ostream &operator<<(std::ostream &os, const ProcessingElement<INPUT_T, OUTPUT_T, ACC_T> &pe)
{
    os << "PE - stationary: " << pe.in_ctrl.stationary << ", propagate: " << pe.in_ctrl.propagate << " ";
    os << "\tIN  a:" << pe.in_a << " b: " << pe.in_b << " d: " << pe.in_d << " ";
    os << "\t  c1:" << pe.c1 << " c2: " << pe.c2 << " acc_c1: " << pe.acc_c1 << " acc_c2: " << pe.acc_c2 << " ";
    os << "\tOUT a:" << pe.out_a << " b: " << pe.out_b << " c: " << pe.out_c << "\n";
    return os;
}