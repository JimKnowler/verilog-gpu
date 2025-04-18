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

TEST(Matrix, ShouldRotateAroundXAxis)
{
    const FVector4 Point(0.0f, 1.0f, 0.0f, 1.0f);
    const FMatrix44 Matrix = FMatrix44::RotateX(M_PI_2);
    const FVector4 Result = Matrix * Point;
    const FVector4 Expected(0.0f, 0.0f, 1.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldRotateAroundYAxis)
{
    const FVector4 Point(1.0f, 0.0f, 0.0f, 1.0f);
    const FMatrix44 Matrix = FMatrix44::RotateY(M_PI_2);
    const FVector4 Result = Matrix * Point;
    const FVector4 Expected(0.0f, 0.0f, -1.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

TEST(Matrix, ShouldRotateAroundZAxis)
{
    const FVector4 Point(0.0f, 1.0f, 0.0f, 1.0f);
    const FMatrix44 Matrix = FMatrix44::RotateZ(M_PI_2);
    const FVector4 Result = Matrix * Point;
    const FVector4 Expected(-1.0f, 0.0f, 0.0f, 1.0f);
    HelperExpectEq(Expected, Result);
}

// TODO: Translate
// TODO: projection (with divide by W for homogenous co-ordinates)
// TODO: camera view (does this require inversion?)
// TODO: concatenate matrices
