// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VFixedPointDivide.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"

#define ENABLE_DEBUG_VERBOSE 0
class FixedPointDivide : public ::testing::Test {
public:
    TestBench<VFixedPointDivide> TestBench;

    void HelperTestDivide(float A, float B)
    {    
        auto& Module = TestBench.Module;
        Module.i_a = ToFixedPoint(A);
        Module.i_b = ToFixedPoint(B);
        TestBench.Eval();

        const float ExpectedResult = A / B;
        EXPECT_EQ(Module.o_result, ToFixedPoint(ExpectedResult));

#if ENABLE_DEBUG_VERBOSE
        printf("      A: %6.2f [%u]\n      B: %6.2f [%u]\n    A/B: %6.2f [%u]\n Result: %6.2f [%u]\n", 
            FromFixedPoint(ToFixedPoint(A)), ToFixedPoint(A),
            FromFixedPoint(ToFixedPoint(B)), ToFixedPoint(B),
            FromFixedPoint(ToFixedPoint(ExpectedResult)), ToFixedPoint(ExpectedResult),
            FromFixedPoint(Module.o_result), Module.o_result
        );
#endif
    }
};

TEST_F(FixedPointDivide, ShouldConstructTestBench)
{
    
}

TEST_F(FixedPointDivide, ShouldDivideIntegers)
{
    HelperTestDivide(58.0f, 45.0f);
}

TEST_F(FixedPointDivide, ShouldDivideFractions)
{ 
    HelperTestDivide(0.5f, 0.25f);
}

TEST_F(FixedPointDivide, ShouldDividePositiveAndNegative)
{ 
    //HelperTestDivide(1.5f, -4.25f);
    HelperTestDivide(1.0f, -0.5f);
}

TEST_F(FixedPointDivide, ShouldDivideNegativeAndPositive)
{ 
    HelperTestDivide(-1.5f, 4.25f);
}

TEST_F(FixedPointDivide, ShouldDivideNegativeAndNegative)
{ 
    //HelperTestDivide(-1.5f, -4.25f);
    HelperTestDivide(-1.5f, -3.0f);
}
