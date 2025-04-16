// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVectorAddVector.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "MathTestHelpers.h"

class VectorAddVector : public ::testing::Test {
public:
    TestBench<VVectorAddVector> TestBench;

    void HelperTestVectorAddVector(const FVector4& A, const FVector4& B)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_a, A);    
        HelperSetFixedPointVector(Module.i_b, B);
        
        TestBench.Eval();

        const FVector4 Result = HelperGetFixedPointVector(Module.o_result);
        HelperExpectEq(A + B, Result);        
    }
};

TEST_F(VectorAddVector, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_a.size(), 4);
    EXPECT_EQ(Module.i_b.size(), 4);
    EXPECT_EQ(Module.o_result.size(), 4);
}

TEST_F(VectorAddVector, ShouldAddZeroVectorToZeroVector)
{
    HelperTestVectorAddVector(FVector4::Zero(), FVector4::Zero());
}

TEST_F(VectorAddVector, ShouldAddVectorToZeroVector)
{
    const FVector4 kVector(1.0f, 2.0f, 4.0f, 5.0f);
    HelperTestVectorAddVector(kVector, FVector4::Zero());
}

TEST_F(VectorAddVector, ShouldAddZeroVectorToVector)
{
    const FVector4 kVector(1.0f, 2.0f, 4.0f, 5.0f);
    HelperTestVectorAddVector(FVector4::Zero(), kVector);
}

TEST_F(VectorAddVector, ShouldAddVectorToVector)
{
    const FVector4 kVectorA(1.0f, 2.0f, 4.0f, 5.0f);
    const FVector4 kVectorB(0.3f, 0.35f, 5.2f, 6.9f);
    HelperTestVectorAddVector(kVectorA, kVectorB);
}

