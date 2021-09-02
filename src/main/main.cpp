#include <cstdint>
#include "../test/matmul.h"

int main(void)
{
    UnitTest::matrix();
    UnitTest::request();
    UnitTest::matmul_output();
    UnitTest::matmul_output2();
    UnitTest::matmul_output_twice();
    UnitTest::matmul_weight();
    UnitTest::matmul_weight2();
    UnitTest::matmul_weight_twice();
    UnitTest::matmul_huge_weight();
    UnitTest::matmul_huge_output();
    return 0;
}