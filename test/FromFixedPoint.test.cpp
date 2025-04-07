// GoogleTest
#include <gtest/gtest.h>

#include "FixedPoint.h"

TEST(FromFixedPoint, ShouldSupportZero)
{
    EXPECT_FLOAT_EQ(0.0f, FromFixedPoint(0));
}

TEST(FromFixedPoint, ShouldSupportOne)
{ 
    EXPECT_FLOAT_EQ(1.0f, FromFixedPoint(1 << 8));
}

TEST(FromFixedPoint, ShouldSupportHalf)
{ 
    EXPECT_FLOAT_EQ(0.5f, FromFixedPoint(1 << 7));
}

TEST(FromFixedPoint, ShouldSupportQuarter)
{ 
    EXPECT_FLOAT_EQ(0.25f, FromFixedPoint(1 << 6));
}

TEST(FromFixedPoint, ShouldSupportPi)
{
    EXPECT_NEAR(3.14f, FromFixedPoint(803), 0.005f);
}
