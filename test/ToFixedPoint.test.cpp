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
