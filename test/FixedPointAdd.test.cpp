// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VFixedPointAdd.h"

#include "TestBench.h"
#include "FixedPoint.h"

class FixedPointAdd : public ::testing::Test {
public:
    TestBench<VFixedPointAdd> TestBench;

    void HelperTestAdd(float A, float B) 
    {
        auto& Module = TestBench.Module;
        Module.i_a = ToFixedPoint(A);
        Module.i_b = ToFixedPoint(B);
        TestBench.Eval();
        
        EXPECT_EQ(Module.o_result, ToFixedPoint(A + B));
    }
};

TEST_F(FixedPointAdd, ShouldConstructTestBench)
{
    
}

TEST_F(FixedPointAdd, ShouldAddIntegers)
{
    HelperTestAdd(58.0f, 45.0f);
}

TEST_F(FixedPointAdd, ShouldAddFractions)
{ 
    HelperTestAdd(0.5f, 0.25f);
}

TEST_F(FixedPointAdd, ShouldAddNegativeAndPositive)
{
    HelperTestAdd(-1.5f, 2.25f);
}

TEST_F(FixedPointAdd, ShouldAddPostiveAndNegative)
{
    HelperTestAdd(1.5f, -2.25f);
}

TEST_F(FixedPointAdd, ShouldAddNegativeAndNegative)
{
    HelperTestAdd(-1.5f, -2.25f);
}
