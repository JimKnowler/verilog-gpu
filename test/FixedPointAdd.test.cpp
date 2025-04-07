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
};

TEST_F(FixedPointAdd, ShouldConstructTestBench)
{
    
}

TEST_F(FixedPointAdd, ShouldAddIntegers)
{
    const float kA = 58.0f;
    const float kB = 45.0f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA + kB));
}

TEST_F(FixedPointAdd, ShouldAddFractions)
{ 
    const float kA = 0.5f;
    const float kB = 0.25f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA + kB));
}

TEST_F(FixedPointAdd, ShouldAddNegativeAndPositive)
{
    const float kA = -1.5f;
    const float kB = 2.25f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA + kB));
}

TEST_F(FixedPointAdd, ShouldAddNegativeAndNegative)
{
    const float kA = -1.5f;
    const float kB = -2.25f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA + kB));
}

// TODO: helper function for repeated code
// TODO: add test for max supported values