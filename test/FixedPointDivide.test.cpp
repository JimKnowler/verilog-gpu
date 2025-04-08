// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VFixedPointDivide.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"

class FixedPointDivide : public ::testing::Test {
public:
    TestBench<VFixedPointDivide> TestBench;

    void HelperTestDivide(float A, float B)
    {    
        auto& Module = TestBench.Module;
        Module.i_a = ToFixedPoint(A);
        Module.i_b = ToFixedPoint(B);
        TestBench.Eval();

        EXPECT_EQ(Module.o_result, ToFixedPoint(A / B));
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
    HelperTestDivide(1.5f, -4.25f);
}

TEST_F(FixedPointDivide, ShouldDivideNegativeAndPositive)
{ 
    HelperTestDivide(-1.5f, 4.25f);
}

TEST_F(FixedPointDivide, ShouldDivideNegativeAndNegative)
{ 
    HelperTestDivide(-1.5f, -4.25f);
}
