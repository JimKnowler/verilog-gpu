// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVectorDivideScalar.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "gpu/Matrix44.h"
#include "MathTestHelpers.h"

class VectorDivideScalar : public ::testing::Test {
public:
    TestBench<VVectorDivideScalar> TestBench;

    void HelperTestVectorDivideScalar(const FVector4& A, const float Scalar)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_a, A);    
        Module.i_scalar = ToFixedPoint(Scalar);
        
        TestBench.Eval();

        const FVector4 Result = HelperGetFixedPointVector(Module.o_result);
        HelperExpectEq(A / Scalar, Result);
    }
};

TEST_F(VectorDivideScalar, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_a.size(), 4);
    EXPECT_EQ(Module.o_result.size(), 4);
}

TEST_F(VectorDivideScalar, ShouldZeroByOne)
{
    HelperTestVectorDivideScalar(FVector4::Zero(), 1.0f);
}

TEST_F(VectorDivideScalar, ShouldDivideByOne)
{
    HelperTestVectorDivideScalar(FVector4(1.0f, 2.0f, 3.0f, 4.0f), 1.0f);
}

TEST_F(VectorDivideScalar, ShouldDivideByTwo)
{
    HelperTestVectorDivideScalar(FVector4(1.0f, 2.0f, 3.0f, 4.0f), 2.0f);
}

TEST_F(VectorDivideScalar, ShouldDivideByHalf)
{
    HelperTestVectorDivideScalar(FVector4(1.0f, 2.0f, 3.0f, 4.0f), 0.5f);
}

TEST_F(VectorDivideScalar, ShouldDivideByMinusTwo)
{
    HelperTestVectorDivideScalar(FVector4(1.0f, 2.0f, 3.0f, 4.0f), -2.0f);
}
