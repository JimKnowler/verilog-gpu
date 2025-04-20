// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VTriangleBackFaceCull.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "MathTestHelpers.h"
#include "gpu/VerilatorHelpers.h"

class TriangleBackFaceCull : public ::testing::Test {
public:
    TestBench<VTriangleBackFaceCull> TestBench;

    void HelperTestTriangleBackFaceCull(const FVector4& v1, const FVector4& v2, const FVector4& v3, const int Expected)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_v1, v1);    
        HelperSetFixedPointVector(Module.i_v2, v2);
        HelperSetFixedPointVector(Module.i_v3, v3);
        
        TestBench.Eval();

        const float CrossProduct = (v1 - v2).CrossProduct2D(v3 - v2);
        const int ResultC = (CrossProduct >= 0.0f) ? 1 : 0;
        const int ResultVerilog = Module.o_result;

        EXPECT_EQ(Expected, ResultC);
        EXPECT_EQ(Expected, ResultVerilog);
    }
};

TEST_F(TriangleBackFaceCull, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_v1.size(), 4);
    EXPECT_EQ(Module.i_v2.size(), 4);
    EXPECT_EQ(Module.i_v3.size(), 4);
}

TEST_F(TriangleBackFaceCull, ShouldCullTrianglePointingAwayFromCamera)
{
    // anti-clockwise winding in screenspace
    HelperTestTriangleBackFaceCull(
        FVector4(10.0f, 0.0f, 0.0f, 1.0f),
        FVector4(0.0f, 0.0f, 0.0f, 1.0f),
        FVector4(5.0f, 5.0f, 0.0f, 1.0f),
        true
    );
}

TEST_F(TriangleBackFaceCull, ShouldCullTriangleSideOnToCameraVertically)
{
    // screenspace vertices have matching x
    HelperTestTriangleBackFaceCull(
        FVector4(0.0f, 0.0f, 0.0f, 1.0f),
        FVector4(0.0f, 5.0f, 0.0f, 1.0f),
        FVector4(0.0f, 10.0f, 0.0f, 1.0f),
        true
    );
}

TEST_F(TriangleBackFaceCull, ShouldCullTriangleSideOnToCameraHorizontally)
{
    // screenspace vertices have matching y
    HelperTestTriangleBackFaceCull(
        FVector4(0.0f, 0.0f, 0.0f, 1.0f),
        FVector4(5.0f, 0.0f, 0.0f, 1.0f),
        FVector4(10.0f, 0.0f, 0.0f, 1.0f),
        true
    );
}

TEST_F(TriangleBackFaceCull, ShouldNotCullTrianglePointingTowardsCamera)
{
    // clockwise winding in screenspace
    HelperTestTriangleBackFaceCull(
        FVector4(0.0f, 0.0f, 0.0f, 1.0f),
        FVector4(10.0f, 0.0f, 0.0f, 1.0f),
        FVector4(5.0f, 5.0f, 0.0f, 1.0f),
        false
    );
}
