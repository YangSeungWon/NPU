#ifndef PROCESSING_ELEMENT_H_
#define PROCESSING_ELEMENT_H_

#include <cstdint>

enum class Stationary
{
    output,
    weight
};

template <typename INPUT_T, typename OUTPUT_T, typename ACC_T>
class ProcessingElement
{
public:
    explicit ProcessingElement(Stationary stationary, uint32_t latency)
        : stationary(_stationary), latency(_latency) {}
    void calculate();
    static Stationary stationary;
    static uint32_t latency;

private:
    INPUT_T  in_a;
    OUTPUT_T in_b;
    OUTPUT_T in_d;
    INPUT_T  out_a;
    OUTPUT_T out_b;
    OUTPUT_T out_c;
    ACC_T    acc_c;
};

#endif
