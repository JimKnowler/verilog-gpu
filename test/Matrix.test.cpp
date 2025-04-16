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

TEST(Matrix, ShouldMultiplyByIdentity)
{
    FMatrix44 Matrix = HelperMakeTestMatrix();
    FMatrix44 Result = Matrix * FMatrix44::Identity();
    HelperExpectEq(Matrix, Result);
}

TEST(Matrix, ShouldMultiplyByZero)
{
    FMatrix44 Matrix = HelperMakeTestMatrix();
    FMatrix44 Result = Matrix * FMatrix44::Zero();
    HelperExpectEq(FMatrix44::Zero(), Result);
}

// TODO: rotate around X axis
// TODO: rotate around Y axis
// TODO: rotate around Z axis
// TODO: Translate
// TODO: projection (with divide by W for homogenous co-ordinates)
// TODO: camera view (does this require inversion?)
// TODO: concatenate matrices
