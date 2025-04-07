// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VFixedPointSub.h"

#include "TestBench.h"
#include "FixedPoint.h"

class FixedPointSub : public ::testing::Test {
public:
    TestBench<VFixedPointSub> TestBench;
};

TEST_F(FixedPointSub, ShouldConstructTestBench)
{
    
}

TEST_F(FixedPointSub, ShouldAddIntegers)
{
    const float kA = 58.0f;
    const float kB = 45.0f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA - kB));
}

TEST_F(FixedPointSub, ShouldAddFractions)
{ 
    const float kA = 0.5f;
    const float kB = 0.25f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA - kB));
}

// TODO negative values, negative-positive, positive-negative