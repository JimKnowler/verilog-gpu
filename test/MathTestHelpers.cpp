#include "MathTestHelpers.h"

// GoogleTest
#include <gtest/gtest.h>

#include "gpu/FixedPoint.h"

namespace
{
    constexpr float kLambda = 0.005f;
}

void HelperExpectEq(const float Expected, const float Actual)
{
    EXPECT_NEAR(Expected, Actual, kLambda);
}

void HelperExpectEq(const FVector4 &Expected, const FVector4 &Actual)
{
    EXPECT_NEAR(Expected.X, Actual.X, kLambda);
    EXPECT_NEAR(Expected.Y, Actual.Y, kLambda);
    EXPECT_NEAR(Expected.Z, Actual.Z, kLambda);
    EXPECT_NEAR(Expected.W, Actual.W, kLambda);
}

void HelperExpectEq(const FMatrix44& Expected, const FMatrix44& Actual)
{
    {
        SCOPED_TRACE("Row 0");
        HelperExpectEq(Expected[0], Actual[0]);
    }

    {
        SCOPED_TRACE("Row 1");
        HelperExpectEq(Expected[1], Actual[1]);
    }

    {
        SCOPED_TRACE("Row 2");
        HelperExpectEq(Expected[2], Actual[2]);
    }

    {
        SCOPED_TRACE("Row 3");
        HelperExpectEq(Expected[3], Actual[3]);
    }
}

void HelperSetFixedPointVector(VlWide<4UL>& FixedPointVector, const FVector4& Vector)
{        
    FixedPointVector.at(0) = ToFixedPoint(Vector.X);
    FixedPointVector.at(1) = ToFixedPoint(Vector.Y);
    FixedPointVector.at(2) = ToFixedPoint(Vector.Z);
    FixedPointVector.at(3) = ToFixedPoint(Vector.W);
}

FVector4 HelperGetFixedPointVector(const VlWide<4UL>& FixedPointVector)
{
    FVector4 Result;
    Result.X = FromFixedPoint(FixedPointVector.at(0));
    Result.Y = FromFixedPoint(FixedPointVector.at(1));
    Result.Z = FromFixedPoint(FixedPointVector.at(2));
    Result.W = FromFixedPoint(FixedPointVector.at(3));

    return Result;
}

void HelperSetFixedPointMatrix(VlWide<16UL>& FixedPointMatrix, const FMatrix44& Matrix)
{        
    for (int i=0; i<4; i++) 
    {
        const FVector4& Row = Matrix.Rows[i];
        const int Offset = i * 4;
        
        FixedPointMatrix.at(Offset + 0) = ToFixedPoint(Row.X);
        FixedPointMatrix.at(Offset + 1) = ToFixedPoint(Row.Y);
        FixedPointMatrix.at(Offset + 2) = ToFixedPoint(Row.Z);
        FixedPointMatrix.at(Offset + 3) = ToFixedPoint(Row.W);
    }
}

float DegreesToRadians(float Degrees)
{
    float Radians = Degrees * 2.0f * M_PI / 360.0f;

    return Radians;
}
