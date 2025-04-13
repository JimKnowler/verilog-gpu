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
        EXPECT_NEAR(Expected.X, Actual.X, kFixedPointLambda);
        EXPECT_NEAR(Expected.Y, Actual.Y, kFixedPointLambda);
        EXPECT_NEAR(Expected.Z, Actual.Z, kFixedPointLambda);
        EXPECT_NEAR(Expected.W, Actual.W, kFixedPointLambda);
    }

    void HelperTestMultiplyVector(const FMatrix44& Matrix, const FVector4& Vector, const FVector4& Expected)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointMatrix(Module.i_matrix, Matrix);    
        HelperSetFixedPointVector(Module.i_vector, Vector);
        
        TestBench.Eval();

        const FVector4 Result = HelperGetFixedPointVector(Module.o_vector);
        HelperExpectEq(Result, Expected);
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
    const FVector4 kVector(1.0f, 2.0f, 3.0f, 4.0f);

    HelperTestMultiplyVector(FMatrix44::Zero(), kVector, FVector4::Zero());
}

TEST_F(MatrixMultiplyVector, ShouldMultiplyByIdentity)
{
    const FVector4 kVector(1.0f, 2.0f, 3.0f, 4.0f);

    HelperTestMultiplyVector(FMatrix44::Identity(), kVector, kVector);
}
