// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VMatrixMultiplyVector.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "gpu/Matrix.h"

class MatrixMultiplyVector : public ::testing::Test {
public:
    TestBench<VMatrixMultiplyVector> TestBench;

    void HelperSetFixedPointVector(VlWide<4UL>& FixedPointVector, const FVector4& Vector)
    {        
        FixedPointVector.at(0) = ToFixedPoint(Vector.X);
        FixedPointVector.at(1) = ToFixedPoint(Vector.Y);
        FixedPointVector.at(2) = ToFixedPoint(Vector.Z);
        FixedPointVector.at(3) = ToFixedPoint(Vector.W);
    }

    void HelperSetFixedPointMatrix(VlWide<16UL>& FixedPointMatrix, const FMatrix44& Matrix)
    {        
        for (int i=0; i<4; i++) 
        {
            const FVector4& Row = Matrix.Rows[i];
            const int Offset = i * 4;
            
            FixedPointMatrix.at(Offset + 0) = ToFixedPoint(Row.X);
            FixedPointMatrix.at(Offset + 1) = ToFixedPoint(Row.Y);
            FixedPointMatrix.at(Offset + 2) = ToFixedPoint(Row.Z);
            FixedPointMatrix.at(Offset + 3) = ToFixedPoint(Row.W);
        }
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
};

TEST_F(MatrixMultiplyVector, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_matrix.size(), 4 * 4);
    EXPECT_EQ(Module.i_vector.size(), 4);
    EXPECT_EQ(Module.o_vector.size(), 4);
}

TEST_F(MatrixMultiplyVector, ShouldMultiplyByZero)
{
    auto& Module = TestBench.Module;

    HelperSetFixedPointMatrix(Module.i_matrix, FMatrix44::Zero());    
    
    const FVector4 kVector(1.0f, 2.0f, 3.0f, 4.0f);
    HelperSetFixedPointVector(Module.i_vector, kVector);
    
    TestBench.Eval();

    const FVector4 Result = HelperGetFixedPointVector(Module.o_vector);
    const FVector4 Expected = FVector4::Zero();
    HelperExpectEq(Result, Expected);
}

TEST_F(MatrixMultiplyVector, ShouldMultiplyByIdentity)
{
    auto& Module = TestBench.Module;

    HelperSetFixedPointMatrix(Module.i_matrix, FMatrix44::Identity());    
    
    const FVector4 kVector(1.0f, 2.0f, 3.0f, 4.0f);
    HelperSetFixedPointVector(Module.i_vector, kVector);

    TestBench.Eval();

    const FVector4 Result = HelperGetFixedPointVector(Module.o_vector);
    HelperExpectEq(Result, kVector);
}
