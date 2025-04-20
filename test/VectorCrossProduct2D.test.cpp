
// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVectorCrossProduct2D.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "MathTestHelpers.h"
#include "gpu/VerilatorHelpers.h"

class VectorCrossProduct2D : public ::testing::Test {
public:
    TestBench<VVectorCrossProduct2D> TestBench;

    void HelperTestVectorCrossProduct2D(const FVector4& A, const FVector4& B)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_a, A);    
        HelperSetFixedPointVector(Module.i_b, B);
        
        TestBench.Eval();

        const float Result = FromFixedPoint(Module.o_result);
        HelperExpectEq(A.CrossProduct2D(B), Result);        
    }
};

TEST_F(VectorCrossProduct2D, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_a.size(), 4);
    EXPECT_EQ(Module.i_b.size(), 4);
}

TEST_F(VectorCrossProduct2D, ShouldCrossXAxisAndYAxis)
{
    HelperTestVectorCrossProduct2D(FVector4::XAxis(), FVector4::YAxis());
}

TEST_F(VectorCrossProduct2D, ShouldCrossYAxisAndZAxis)
{
    HelperTestVectorCrossProduct2D(FVector4::YAxis(), FVector4::ZAxis());
}

TEST_F(VectorCrossProduct2D, ShouldCrossXAxisAndZAxis)
{
    HelperTestVectorCrossProduct2D(FVector4::XAxis(), FVector4::ZAxis());
}
