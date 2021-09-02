#include <iostream>
#include "processing_element.h"

std::ostream &operator<<(std::ostream &os, const Stationary &stationary)
{
    if (stationary == Stationary::output)
        os << 'O';
    else if (stationary == Stationary::weight)
        os << 'W';
    return os;
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>::ProcessingElement() :
    in_a(0),
    in_b(0),
    in_d(0),
    in_ctrl({Stationary::weight, false}),
    out_a(0),
    out_b(0),
    out_c(0),
    out_ctrl({Stationary::weight, false}),
    acc_c1(0),
    acc_c2(0)
{}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>::calculate()
{
    out_a = in_a; // pass input vector to the next PE(right)
    out_ctrl = in_ctrl; // pass control parameter to the bottom PE
    switch (in_ctrl.stationary)
    {
    case Stationary::weight:
        switch (in_ctrl.propagate)
        {
        case true:
            out_c = c1; // propagate weight
            out_b = (OUTPUT_T)(in_a * (INPUT_T)c2) + in_b; // MAC and pass result
            c1 = in_d; // preloading weight
            break;
        case false:
            out_c = c2; // propagate weight
            out_b = (OUTPUT_T)(in_a * (INPUT_T)c1) + in_b; // MAC and pass result
            c2 = in_d; // preloading weight
            break;
        }
        break;
    case Stationary::output:
        switch (in_ctrl.propagate)
        {
        case true: 
            out_c = (OUTPUT_T)acc_c1; // reveal result
            out_b = in_b; // pass weight to the next PE(below)
            acc_c2 = (ACC_T)(in_a * (INPUT_T)in_b) + acc_c2; // MAC
            acc_c1 = (ACC_T)in_d; // prepare bias
            break;
        case false:
            out_c = (OUTPUT_T)acc_c2; // reveal result
            out_b = in_b; // pass weight to the next PE(below)
            acc_c1 = (ACC_T)(in_a * (INPUT_T)in_b) + acc_c1; // MAC
            acc_c2 = (ACC_T)in_d; // prepare bias
            break;
        }
        break;
    }
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>::input_a(INPUT_T data)
{
    in_a = data;
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>::input_b(INPUT_T data)
{
    in_b = (OUTPUT_T)data;
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>::input_b(OUTPUT_T data)
{
    in_b = data;
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>::input_d(INPUT_T data)
{
    in_d = (OUTPUT_T)data;
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>::input_d(OUTPUT_T data)
{
    in_d = data;
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
void ProcessingElement<INPUT_T, OUTPUT_T, ACC_T>::input_ctrl(control_t data)
{
    in_ctrl = data;
}

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
std::ostream &operator<<(std::ostream &os, const ProcessingElement<INPUT_T, OUTPUT_T, ACC_T> &pe)
{
    os << "PE - stationary: " << pe.in_ctrl.stationary << ", propagate: " << pe.in_ctrl.propagate << " ";
    os << "\tIN  a:" << pe.in_a << " b: " << pe.in_b << " d: " << pe.in_d << " ";
    os << "\t  c1:" << pe.c1 << " c2: " << pe.c2 << " acc_c1: " << pe.acc_c1 << " acc_c2: " << pe.acc_c2 << " ";
    os << "\tOUT a:" << pe.out_a << " b: " << pe.out_b << " c: " << pe.out_c << "\n";
    return os;
}