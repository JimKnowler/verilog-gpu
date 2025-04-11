// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVectorAddVector.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "gpu/Matrix.h"

class VectorAddVector : public ::testing::Test {
public:
    TestBench<VVectorAddVector> TestBench;

    void HelperSetFixedPointVector(VlWide<4UL>& FixedPointVector, const FVector4& Vector)
    {        
        FixedPointVector.at(0) = ToFixedPoint(Vector.X);
        FixedPointVector.at(1) = ToFixedPoint(Vector.Y);
        FixedPointVector.at(2) = ToFixedPoint(Vector.Z);
        FixedPointVector.at(3) = ToFixedPoint(Vector.W);
    }

    FVector4 HelperGetFixedPointVector(const VlWide<4UL>& FixedPointVector)
    {
        FVector4 Result;
        Result.X = FromFixedPoint(FixedPointVector.at(0));
        Result.Y = FromFixedPoint(FixedPointVector.at(1));
        Result.Z = FromFixedPoint(FixedPointVector.at(2));
        Result.W = FromFixedPoint(FixedPointVector.at(3));

        return Result;
    }

    void HelperExpectEq(const FVector4& Expected, const FVector4& Actual) 
    {
        constexpr float kLambda = 0.005f;
        EXPECT_NEAR(Expected.X, Actual.X, kLambda);
        EXPECT_NEAR(Expected.Y, Actual.Y, kLambda);
        EXPECT_NEAR(Expected.Z, Actual.Z, kLambda);
        EXPECT_NEAR(Expected.W, Actual.W, kLambda);
    }

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

