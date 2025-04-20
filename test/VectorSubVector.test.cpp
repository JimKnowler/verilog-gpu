// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVectorSubVector.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "MathTestHelpers.h"
#include "gpu/VerilatorHelpers.h"

class VectorSubVector : public ::testing::Test {
public:
    TestBench<VVectorSubVector> TestBench;

    void HelperTestVectorSubVector(const FVector4& A, const FVector4& B)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_a, A);    
        HelperSetFixedPointVector(Module.i_b, B);
        
        TestBench.Eval();

        const FVector4 Result = HelperGetFixedPointVector(Module.o_result);
        HelperExpectEq(A - B, Result);        
    }
};

TEST_F(VectorSubVector, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_a.size(), 4);
    EXPECT_EQ(Module.i_b.size(), 4);
    EXPECT_EQ(Module.o_result.size(), 4);
}

TEST_F(VectorSubVector, ShouldSubZeroVectorAndZeroVector)
{
    HelperTestVectorSubVector(FVector4::Zero(), FVector4::Zero());
}

TEST_F(VectorSubVector, ShouldSubVectorAndZeroVector)
{
    const FVector4 kVector(1.0f, 2.0f, 4.0f, 5.0f);
    HelperTestVectorSubVector(kVector, FVector4::Zero());
}

TEST_F(VectorSubVector, ShouldSubZeroVectorAndVector)
{
    const FVector4 kVector(1.0f, 2.0f, 4.0f, 5.0f);
    HelperTestVectorSubVector(FVector4::Zero(), kVector);
}

TEST_F(VectorSubVector, ShouldSubVectorAndVector)
{
    const FVector4 kVectorA(1.0f, 2.0f, 4.0f, 5.0f);
    const FVector4 kVectorB(0.3f, 0.35f, 5.2f, 6.9f);
    HelperTestVectorSubVector(kVectorA, kVectorB);
}

