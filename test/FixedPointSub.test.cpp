// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VFixedPointSub.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"

class FixedPointSub : public ::testing::Test {
public:
    TestBench<VFixedPointSub> TestBench;

    void HelperTestSub(float A, float B)
    {
        auto& Module = TestBench.Module;
        Module.i_a = ToFixedPoint(A);
        Module.i_b = ToFixedPoint(B);
        TestBench.Eval();

        EXPECT_EQ(Module.o_result, ToFixedPoint(A - B));
    }
};

TEST_F(FixedPointSub, ShouldConstructTestBench)
{
    
}

TEST_F(FixedPointSub, ShouldSubIntegers)
{
    HelperTestSub(58.0f, 45.0f);
}

TEST_F(FixedPointSub, ShouldSubFractions)
{ 
    HelperTestSub(0.5f, 0.25f);
}

TEST_F(FixedPointSub, ShouldSubPositiveAndNegative)
{ 
    HelperTestSub(1.5f, -2.25f);
}

TEST_F(FixedPointSub, ShouldSubNegativeAndPositive)
{ 
    HelperTestSub(-1.5f, 2.25f);
}

TEST_F(FixedPointSub, ShouldSubNegativeAndNegative)
{ 
    HelperTestSub(-1.5f, -2.25f);
}

TEST_F(FixedPointSub, ShouldGetNegativeResult)
{
    HelperTestSub(1.5f, 2.25f);
}
