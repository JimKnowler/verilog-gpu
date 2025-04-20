// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVectorCrossProduct.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "MathTestHelpers.h"
#include "gpu/VerilatorHelpers.h"

class VectorCrossProduct : public ::testing::Test {
public:
    TestBench<VVectorCrossProduct> TestBench;

    void HelperTestVectorCrossProduct(const FVector4& A, const FVector4& B)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_a, A);    
        HelperSetFixedPointVector(Module.i_b, B);
        
        TestBench.Eval();

        const FVector4 Result = HelperGetFixedPointVector(Module.o_result);
        HelperExpectEq(A.CrossProduct(B), Result);        
    }
};

TEST_F(VectorCrossProduct, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_a.size(), 4);
    EXPECT_EQ(Module.i_b.size(), 4);
    EXPECT_EQ(Module.o_result.size(), 4);
}

TEST_F(VectorCrossProduct, ShouldCrossXAxisAndYAxis)
{
    HelperTestVectorCrossProduct(FVector4::XAxis(), FVector4::YAxis());
}

TEST_F(VectorCrossProduct, ShouldCrossYAxisAndZAxis)
{
    HelperTestVectorCrossProduct(FVector4::YAxis(), FVector4::ZAxis());
}

TEST_F(VectorCrossProduct, ShouldCrossXAxisAndZAxis)
{
    HelperTestVectorCrossProduct(FVector4::XAxis(), FVector4::ZAxis());
}
