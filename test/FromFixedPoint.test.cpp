// GoogleTest
#include <gtest/gtest.h>

#include "gpu/FixedPoint.h"

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
    // fixed point representation of pi, when using fractionwidth = 16
    const uint32_t kPiFractionWidth16 = 205783;

    EXPECT_NEAR(3.14f, FromFixedPoint(kPiFractionWidth16 << (kFractionWidth - 16)), 0.005f);
}
