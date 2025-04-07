// GoogleTest
#include <gtest/gtest.h>

#include "FixedPoint.h"

TEST(ToFixedPoint, ShouldSupportZero)
{
    EXPECT_EQ(0, ToFixedPoint(0.0f));
}

TEST(ToFixedPoint, ShouldSupportOne)
{ 
    EXPECT_EQ(1 << 8, ToFixedPoint(1.0f));
}

TEST(ToFixedPoint, ShouldSupportHalf)
{ 
    EXPECT_EQ(1 << 7, ToFixedPoint(0.5f));
}

TEST(ToFixedPoint, ShouldSupportQuarter)
{ 
    EXPECT_EQ(1 << 6, ToFixedPoint(0.25f));
}

TEST(ToFixedPoint, ShouldSupportPi)
{
    EXPECT_EQ(803, ToFixedPoint(3.14f));
}


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
