// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VMatrixMultiplyVector.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "gpu/Matrix44.h"
#include "MathTestHelpers.h"

class MatrixMultiplyVector : public ::testing::Test {
public:
    TestBench<VMatrixMultiplyVector> TestBench;

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
