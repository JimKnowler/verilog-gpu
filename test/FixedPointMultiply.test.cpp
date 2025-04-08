// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VFixedPointMultiply.h"

#include "TestBench.h"
#include "FixedPoint.h"

class FixedPointMultiply : public ::testing::Test {
public:
    TestBench<VFixedPointMultiply> TestBench;

    void HelperTestMultiply(float A, float B)
    {    
        auto& Module = TestBench.Module;
        Module.i_a = ToFixedPoint(A);
        Module.i_b = ToFixedPoint(B);
        TestBench.Eval();

        EXPECT_EQ(Module.o_result, ToFixedPoint(A * B));
    }
};

TEST_F(FixedPointMultiply, ShouldConstructTestBench)
{
    
}

TEST_F(FixedPointMultiply, ShouldMultiplyIntegers)
{
    HelperTestMultiply(58.0f, 45.0f);
}

TEST_F(FixedPointMultiply, ShouldMultiplyFractions)
{ 
    HelperTestMultiply(0.5f, 0.25f);
}

TEST_F(FixedPointMultiply, ShouldMultiplyPositiveAndNegative)
{ 
    HelperTestMultiply(1.5f, -4.25f);
}

TEST_F(FixedPointMultiply, ShouldMultiplyNegativeAndNegative)
{ 
    HelperTestMultiply(-1.5f, -4.25f);
}
