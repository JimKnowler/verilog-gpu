// GoogleTest
#include <gtest/gtest.h>

#include "gpu/FixedPoint.h"

TEST(ToFixedPoint, ShouldSupportZero)
{
    EXPECT_EQ(0, ToFixedPoint(0.0f));
}

TEST(ToFixedPoint, ShouldSupportOne)
{ 
    EXPECT_EQ(1 << kFractionWidth, ToFixedPoint(1.0f));
}

TEST(ToFixedPoint, ShouldSupportHalf)
{ 
    EXPECT_EQ(1 << (kFractionWidth-1), ToFixedPoint(0.5f));
}

TEST(ToFixedPoint, ShouldSupportQuarter)
{ 
    EXPECT_EQ(1 << (kFractionWidth-2), ToFixedPoint(0.25f));
}

TEST(ToFixedPoint, ShouldSupportPi)
{
    const uint32_t kPiFractionWidth17 = 411566;

    EXPECT_EQ(kPiFractionWidth17, ToFixedPoint(3.14f));
}

TEST(ToFixedPoint, ShouldSupportMinusOne)
{
    EXPECT_EQ(-(1 << kFractionWidth), ToFixedPoint(-1.0f));
}
