// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVectorDotProduct.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "gpu/Matrix.h"

class VectorDotProduct : public ::testing::Test {
public:
    TestBench<VVectorDotProduct> TestBench;

    void HelperSetFixedPointVector(VlWide<4UL>& FixedPointVector, const FVector4& Vector)
    {        
        FixedPointVector.at(0) = ToFixedPoint(Vector.X);
        FixedPointVector.at(1) = ToFixedPoint(Vector.Y);
        FixedPointVector.at(2) = ToFixedPoint(Vector.Z);
        FixedPointVector.at(3) = ToFixedPoint(Vector.W);
    }

    void HelperTestVectorDotProduct(const FVector4& A, const FVector4& B, const float Expected)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_a, A);    
        HelperSetFixedPointVector(Module.i_b, B);
        
        TestBench.Eval();

        const float Result = FromFixedPoint(Module.o_scalar);
        const float kLambda = 0.005f;

        EXPECT_NEAR(Expected, Result, kLambda);
    }
};

TEST_F(VectorDotProduct, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_a.size(), 4);
    EXPECT_EQ(Module.i_b.size(), 4);
}

TEST_F(VectorDotProduct, ShouldDotProductWithSameVector)
{
    const FVector4 kVector(1.0f, 2.0f, 4.0f, 5.0f);
    HelperTestVectorDotProduct(kVector, kVector, kVector.Dot(kVector));
}

TEST_F(VectorDotProduct, ShouldDotProductWithZero)
{
    const FVector4 kVector(1.0f, 2.0f, 4.0f, 5.0f);
    HelperTestVectorDotProduct(kVector, FVector4::Zero(), 0.0f);
}

TEST_F(VectorDotProduct, ShouldDotProductZeroWithZero)
{
    const FVector4 kVector = FVector4::Zero();
    HelperTestVectorDotProduct(kVector, kVector, 0.0f);
}

TEST_F(VectorDotProduct, ShouldDotProduct)
{
    const FVector4 kVectorA(1.0f, 2.0f, 4.0f, 5.0f);
    const FVector4 kVectorB(7.0f, 4.0f, 6.0f, 1.0f);
    HelperTestVectorDotProduct(kVectorA, kVectorB, kVectorA.Dot(kVectorB));
}