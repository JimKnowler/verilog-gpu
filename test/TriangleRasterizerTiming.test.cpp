// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VTriangleRasterizerTiming.h"

#include "ClockedTestBench.h"
#include "gpu/FixedPoint.h"
#include "MathTestHelpers.h"
#include "gpu/VerilatorHelpers.h"

class TriangleRasterizerTiming : public ::testing::Test {
public:
    ClockedTestBench<VTriangleRasterizerTiming> TestBench;
};

TEST_F(TriangleRasterizerTiming, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_v1.size(), 4);
    EXPECT_EQ(Module.i_v2.size(), 4);
    EXPECT_EQ(Module.i_v3.size(), 4);
}

TEST_F(TriangleRasterizerTiming, ShouldResetToIdle)
{
    auto& Module = TestBench.Module;

    // Module should ignore the AABB formed by these vertices
    HelperSetFixedPointVector(Module.i_v1, FVector4(50, 50, 0, 1));
    HelperSetFixedPointVector(Module.i_v2, FVector4(200, 100, 0, 1));
    HelperSetFixedPointVector(Module.i_v3, FVector4(50, 120, 0, 1));

    TestBench.Reset();

    for (int i=0; i<10; i++)
    {
        EXPECT_EQ(1, Module.o_ready);
        EXPECT_EQ(0, Module.o_valid);
        EXPECT_EQ(0, Module.o_x);
        EXPECT_EQ(0, Module.o_y);
    }
}

TEST_F(TriangleRasterizerTiming, ShouldIterateOverTriangleAABB)
{
    TestBench.Reset();

    auto& Module = TestBench.Module;

    HelperSetFixedPointVector(Module.i_v1, FVector4(25, 50, 0, 1));
    HelperSetFixedPointVector(Module.i_v2, FVector4(200, 100, 0, 1));
    HelperSetFixedPointVector(Module.i_v3, FVector4(50, 120, 0, 1));

    Module.i_start = 1;
    
    TestBench.Tick();

    Module.i_start = 0;

    for (int y = 50; y <= 120; y++)
    {
        for (int x = 25; x <= 200; x++)
        {
            char Buffer[32];
            snprintf(Buffer, sizeof(Buffer), "x=%d y=%d", x, y);
            SCOPED_TRACE(Buffer);

            ASSERT_EQ(0, Module.o_ready);
            ASSERT_EQ(1, Module.o_valid);

            ASSERT_EQ(x, Module.o_x);
            ASSERT_EQ(y, Module.o_y);

            TestBench.Tick();            
        }
    }

    ASSERT_EQ(1, Module.o_ready);
    ASSERT_EQ(0, Module.o_valid);
    ASSERT_EQ(0, Module.o_x);
    ASSERT_EQ(0, Module.o_y);
}
