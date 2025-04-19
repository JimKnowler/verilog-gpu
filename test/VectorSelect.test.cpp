
// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVectorSelect.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "gpu/Matrix44.h"
#include "MathTestHelpers.h"
#include "gpu/VerilatorHelpers.h"
#include "gpu/VerilatorHelpers.h"

class VectorSelect : public ::testing::Test {
public:
    TestBench<VVectorSelect> TestBench;

    const FVector4 kTestVector = FVector4(1.0f, 2.0f, 3.0f, 4.0f);

    void HelperTestVectorSelect(int Index)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_vector, kTestVector);
        Module.i_index = Index;
        
        TestBench.Eval();

        const float Expected = kTestVector[Index];
        const float Result = FromFixedPoint(Module.o_result);
        HelperExpectEq(Expected, Result);
    }    
};

TEST_F(VectorSelect, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_vector.size(), 4);
}

TEST_F(VectorSelect, ShouldSelectX)
{
    HelperTestVectorSelect(0);
}

TEST_F(VectorSelect, ShouldSelectY)
{
    HelperTestVectorSelect(1);
}

TEST_F(VectorSelect, ShouldSelectZ)
{
    HelperTestVectorSelect(2);
}

TEST_F(VectorSelect, ShouldSelectW)
{
    HelperTestVectorSelect(3);
}
