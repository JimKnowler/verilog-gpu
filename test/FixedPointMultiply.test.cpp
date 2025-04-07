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
};

TEST_F(FixedPointMultiply, ShouldConstructTestBench)
{
    
}

TEST_F(FixedPointMultiply, ShouldMultiplyIntegers)
{
    const float kA = 58.0f;
    const float kB = 45.0f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA * kB));
}

TEST_F(FixedPointMultiply, ShouldMultiplyFractions)
{ 
    const float kA = 0.5f;
    const float kB = 0.25f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA * kB));
}

TEST_F(FixedPointMultiply, ShouldMultiplyPositiveAndNegative)
{ 
    const float kA = 1.5f;
    const float kB = -4.25f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA * kB));
}

TEST_F(FixedPointMultiply, ShouldMultiplyNegativeAndNegative)
{ 
    const float kA = -1.5f;
    const float kB = -4.25f;

    auto& Module = TestBench.Module;
    Module.i_a = ToFixedPoint(kA);
    Module.i_b = ToFixedPoint(kB);
    
    TestBench.Eval();
    EXPECT_EQ(Module.o_result, ToFixedPoint(kA * kB));
}

// TODO: helper function for repeated code
// TODO: add test for max supported values
