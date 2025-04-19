// GoogleTest
#include <gtest/gtest.h>

#include "gpu/FixedPoint.h"
#include "MathTestHelpers.h"

TEST(FromFixedPoint, ShouldSupportZero)
{
    EXPECT_FLOAT_EQ(0.0f, FromFixedPoint(0));
}

TEST(FromFixedPoint, ShouldSupportOne)
{ 
    EXPECT_FLOAT_EQ(1.0f, FromFixedPoint(1 << kFractionWidth));
}

TEST(FromFixedPoint, ShouldSupportHalf)
{ 
    EXPECT_FLOAT_EQ(0.5f, FromFixedPoint(1 << (kFractionWidth - 1)));
}

TEST(FromFixedPoint, ShouldSupportQuarter)
{ 
    EXPECT_FLOAT_EQ(0.25f, FromFixedPoint(1 << (kFractionWidth -2)));
}

TEST(FromFixedPoint, ShouldSupportPi)
{
    const uint32_t kPiFractionWidth14 = 51445;

    HelperExpectEq(3.14f, FromFixedPoint(kPiFractionWidth14));
}
