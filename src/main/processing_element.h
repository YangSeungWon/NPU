#ifndef PROCESSING_ELEMENT_H_
#define PROCESSING_ELEMENT_H_

#include <cstdint>
#include <iostream>

enum class Stationary
{
    output,
    weight
};
std::ostream &operator<<(std::ostream &os, const Stationary &stationary);

typedef struct Control
{
    Stationary stationary;
    bool propagate;
} control_t;

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
class ProcessingElement
{
public:
    ProcessingElement();
    void calculate();
    void input_a(INPUT_T data);
    void input_b(INPUT_T data);
    void input_b(OUTPUT_T data);
    void input_d(INPUT_T data);
    void input_d(OUTPUT_T data);
    void input_ctrl(control_t data);
    uint32_t latency;

    INPUT_T in_a;
    OUTPUT_T in_b;
    OUTPUT_T in_d;
    control_t in_ctrl;
    INPUT_T out_a;
    OUTPUT_T out_b;
    OUTPUT_T out_c;
    control_t out_ctrl;

private:
    ACC_T acc_c1;
    ACC_T acc_c2;
    INPUT_T c1;
    INPUT_T c2;
};

#include "processing_element.hpp"
#endif