// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVectorMultiplyScalar.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "gpu/Matrix.h"

class VectorMultiplyScalar : public ::testing::Test {
public:
    TestBench<VVectorMultiplyScalar> TestBench;

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

    void HelperTestVectorMultiplyScalar(const FVector4& A, const float Scalar)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_a, A);    
        Module.i_scalar = ToFixedPoint(Scalar);
        
        TestBench.Eval();

        const FVector4 Result = HelperGetFixedPointVector(Module.o_result);
        HelperExpectEq(A * Scalar, Result);
    }
};

TEST_F(VectorMultiplyScalar, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_a.size(), 4);
}

TEST_F(VectorMultiplyScalar, ShouldMultiplyByZero)
{
    HelperTestVectorMultiplyScalar(FVector4(1.0f, 2.0f, 3.0f, 4.0f), 0.0f);
}

TEST_F(VectorMultiplyScalar, ShouldMultiplyByOne)
{
    HelperTestVectorMultiplyScalar(FVector4(1.0f, 2.0f, 3.0f, 4.0f), 1.0f);
}

TEST_F(VectorMultiplyScalar, ShouldMultiplyByTwo)
{
    HelperTestVectorMultiplyScalar(FVector4(1.0f, 2.0f, 3.0f, 4.0f), 2.0f);
}

TEST_F(VectorMultiplyScalar, ShouldMultiplyByHalf)
{
    HelperTestVectorMultiplyScalar(FVector4(1.0f, 2.0f, 3.0f, 4.0f), 0.5f);
}

TEST_F(VectorMultiplyScalar, ShouldMultiplyByMinusTwo)
{
    HelperTestVectorMultiplyScalar(FVector4(1.0f, 2.0f, 3.0f, 4.0f), -2.0f);
}


