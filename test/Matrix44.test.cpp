// GoogleTest
#include <gtest/gtest.h>

#include "MathTestHelpers.h"

#define UNUSED(_x) (void)(_x)

namespace 
{
    // Create a matrix with a unique value in each element
    FMatrix44 HelperMakeTestMatrix()
    {
        FMatrix44 Matrix;

        for (int i=0; i<4; i++)
        {
            for (int j=0; j<4; j++)
            {
                Matrix[i][j] = static_cast<float>((i * 4) + j);
            }
        }

        return Matrix;
    }
}

TEST(Matrix, ShouldConstruct)
{
    FMatrix44 Matrix;
    UNUSED(Matrix);
}

TEST(Matrix, ShouldSupportAssignment)
{
    FMatrix44 Matrix = HelperMakeTestMatrix();
    HelperExpectEq(HelperMakeTestMatrix(), Matrix);
}

TEST(Matrix, ShouldMultiplyMatrixByIdentityMatrix)
{
    FMatrix44 Matrix = HelperMakeTestMatrix();
    FMatrix44 Result = Matrix * FMatrix44::Identity();
    HelperExpectEq(Matrix, Result);
}

TEST(Matrix, ShouldMultiplyMatrixByZeroMatrix)
{
    FMatrix44 Matrix = HelperMakeTestMatrix();
    FMatrix44 Result = Matrix * FMatrix44::Zero();
    HelperExpectEq(FMatrix44::Zero(), Result);
}

TEST(Matrix, ShouldMultiplyVectorByIdentityMatrix)
{
    const FVector4 Point(1.0f, 2.0f, 3.0f, 4.0f);
    const FVector4 Result = FMatrix44::Identity() * Point;
    HelperExpectEq(Point, Result);
}

TEST(Matrix, ShouldMultiplyVectorByZeroMatrix)
{
    const FVector4 Point(1.0f, 2.0f, 3.0f, 4.0f);
    const FVector4 Result = FMatrix44::Zero() * Point;
    const FVector4 Expected(0.0f, 0.0f, 0.0f, 0.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldRotatePointAroundXAxis)
{
    const FVector4 Point(0.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 Result = FMatrix44::RotateX(M_PI_2) * Point;
    const FVector4 Expected(0.0f, 0.0f, 1.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldRotatePointAroundYAxis)
{
    const FVector4 Point(1.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 Result = FMatrix44::RotateY(M_PI_2) * Point;
    const FVector4 Expected(0.0f, 0.0f, -1.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldRotatePointAroundZAxis)
{
    const FVector4 Point(0.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 Result = FMatrix44::RotateZ(M_PI_2) * Point;
    const FVector4 Expected(-1.0f, 0.0f, 0.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldTranslatePoint)
{
    const FVector4 Point(1.0f, 1.0f, 1.0f, 1.0f);
    const FVector4 Result = FMatrix44::Translation(1.0f, 2.0f, 3.0f) * Point;
    const FVector4 Expected(2.0f, 3.0f, 4.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldConcatenateRotationAndTranslationMatrices)
{
    // Rotate Point, and then translate it
    const FMatrix44 Matrix = FMatrix44::Translation(-1, 0, 0) * FMatrix44::RotateZ(M_PI_2);
    const FVector4 Point(0.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 Result = Matrix * Point;
    const FVector4 Expected(-2.0f, 0.0f, 0.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldLookAt_CenterPoint_FromOrigin)
{
    // Camera
    const FVector4 Eye(0.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 Center(0.0f, 0.0f, -1.0f, 1.0f);
    const FVector4 Up(0.0f, 1.0f, 0.0f, 0.0f);

    // Test Point - use the point that we're looking at
    const FVector4 Point = Center;
    const FVector4 Result = FMatrix44::LookAt(Eye, Center, Up) * Point;
    const FVector4 Expected = Point;
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldLookAt_AboveCenterPoint_FromOrigin)
{
    // Camera
    const FVector4 Eye(0.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 Center(0.0f, 0.0f, -1.0f, 1.0f);
    const FVector4 Up(0.0f, 1.0f, 0.0f, 0.0f);

    // Test Point - a point above the Center
    const FVector4 Point = Center + Up;
    const FVector4 Result = FMatrix44::LookAt(Eye, Center, Up) * Point;
    const FVector4 Expected = Point;
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldLookAt_CenterPoint_FromPointAlongZAxis)
{
    // Camera
    const FVector4 Eye(0.0f, 0.0f, 1.0f, 1.0f);
    const FVector4 Center(0.0f, 0.0f, -1.0f, 1.0f);
    const FVector4 Up(0.0f, 1.0f, 0.0f, 0.0f);

    // Test Point - use the point that we're looking at
    const FVector4 Point = Center;
    const FVector4 Result = FMatrix44::LookAt(Eye, Center, Up) * Point;
    const FVector4 Expected(0.0f, 0.0f, -2.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldLookAt_AboveCenterPoint_FromOrigin_WithRotatedUp)
{
    // Camera
    const FVector4 Eye(0.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 Center(0.0f, 0.0f, -1.0f, 1.0f);
    const FVector4 Up(1.0f, 0.0f, 0.0f, 0.0f);      // rotated UP axis

    // Test Point - a point above the Center
    const FVector4 Point(0.0f, 1.0f, -1.0f, 1.0f);
    const FVector4 Result = FMatrix44::LookAt(Eye, Center, Up) * Point;
    const FVector4 Expected(-1.0f, 0.0f, -1.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldLookAt_ArbitraryPoint_FromArbitraryPoint_WithRotatedUp)
{
    // Camera
    const FVector4 Eye(0.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 Center(1.0f, 1.0f, 0.0f, 1.0f);      // looking along x axis
    const FVector4 Up(0.0f, 0.0f, 1.0f, 0.0f);          // Z axis is up

    const FVector4 Point(2.0f, 4.0f, 6.0f, 1.0f);
    const FVector4 Result = FMatrix44::LookAt(Eye, Center, Up) * Point;
    const FVector4 Expected(-3.0f, 6.0f, -2.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

// TODO: projection (with divide by W for homogenous co-ordinates)
// TODO: concatenate camera and projection matrices
