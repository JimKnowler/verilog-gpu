
// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VVertexTransform.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"
#include "gpu/Matrix44.h"
#include "MathTestHelpers.h"

class VertexTransform : public ::testing::Test {
public:
    TestBench<VVertexTransform> TestBench;

    FMatrix44 HelperMakeMatrix(float ScreenWidth, float ScreenHeight)
    {
        const FVector4 Eye(0, 0, 400, 1);
        const FVector4 Center(0, 0, 0, 1);
        const FVector4 Up(0, 1, 0, 0);
        const FMatrix44 View = FMatrix44::LookAt(Eye, Center, Up);

        const float FOV = DegreesToRadians(45.0f);
        const float Aspect = ScreenWidth / ScreenHeight;
        const float Near = 0.1f;
        const float Far = 1000.0f;
        const FMatrix44 Projection = FMatrix44::Perspective(FOV, Aspect, Near, Far);

        const FMatrix44 Matrix = Projection * View;

        return Matrix;
    }

    void HelperTestVertexTransform(const FVector4& Vertex, const FMatrix44& Matrix, const float ScreenWidth, const float ScreenHeight)
    {
        auto& Module = TestBench.Module;
        HelperSetFixedPointVector(Module.i_vertex, Vertex);
        HelperSetFixedPointMatrix(Module.i_matrix, Matrix);    
        Module.i_screenWidth = ToFixedPoint(ScreenWidth);
        Module.i_screenHeight = ToFixedPoint(ScreenHeight);
        
        TestBench.Eval();

        const FVector4 Result = HelperGetFixedPointVector(Module.o_vertex);
        
        const FVector4 ClipSpace = Matrix * Vertex;
        //printf("  ClipSpace %s\n", ClipSpace.ToString().c_str());

        const FVector4 NDC = ClipSpace / NDC.W;
        //printf("        NDC %s\n", Expected.ToString().c_str());

        FVector4 ScreenSpace = NDC;
        const float kHalfScreenWidth = ScreenWidth * 0.5f;
        const float kHalfScreenHeight = ScreenHeight * 0.5f;
        ScreenSpace.X = (ScreenSpace.X * kHalfScreenWidth) + kHalfScreenWidth; 
        ScreenSpace.Y = (ScreenSpace.Y * kHalfScreenHeight) + kHalfScreenHeight;
        //printf("ScreenSpace %s\n", ScreenSpace.ToString().c_str());

        //printf("     Result %s\n", Result.ToString().c_str());
        
        EXPECT_TRUE(ScreenSpace.Z > -1.0f);
        EXPECT_TRUE(ScreenSpace.Z < 1.0f);
        EXPECT_TRUE(ScreenSpace.IsPoint());

        EXPECT_TRUE(Result.IsPoint());
        EXPECT_GT(Result.Z, -1.0f);
        EXPECT_LT(Result.Z, 1.0f);

        HelperExpectEq(ScreenSpace, Result);
    }
    
};

TEST_F(VertexTransform, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_matrix.size(), 4 * 4);
    EXPECT_EQ(Module.i_vertex.size(), 4);
    EXPECT_EQ(Module.o_vertex.size(), 4);
}

TEST_F(VertexTransform, ShouldTransformVertices)
{
    const FVector4 Vertices[3] = {
        {-50, -60, 0, 1},
        {100, 100, 0, 1},
        {-100, 60, 0, 1}
    };

    const float ScreenWidth = 600.0f;
    const float ScreenHeight = 400.0f;

    const FMatrix44 Matrix = HelperMakeMatrix(ScreenWidth, ScreenHeight);

    for (int i=0; i<3; i++)
    {
        char ScopedTrace[32];
        snprintf(ScopedTrace, sizeof(ScopedTrace), "Vertex [%d]", i);
        
        SCOPED_TRACE(ScopedTrace);

        const FVector4& Vertex = Vertices[i];

        HelperTestVertexTransform(Vertex, Matrix, ScreenWidth, ScreenHeight);
    }
}
