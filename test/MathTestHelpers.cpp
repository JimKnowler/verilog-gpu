#include "MathTestHelpers.h"

// GoogleTest
#include <gtest/gtest.h>

namespace
{
    constexpr float kLambda = 0.01f;
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

float DegreesToRadians(float Degrees)
{
    float Radians = Degrees * 2.0f * M_PI / 360.0f;

    return Radians;
}
