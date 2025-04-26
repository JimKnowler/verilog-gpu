#pragma once

#include <cinttypes>
#include <math.h>

// accuracy for unit tests
constexpr float kFixedPointLambda = 0.005f;

// number of bits used for the fractional part
constexpr int kFractionWidth = 17;

template <int kFraction = kFractionWidth>
uint32_t ToFixedPoint(float Value)
{
    int32_t Result = static_cast<int32_t>(Value * pow(2, kFraction));

    return static_cast<uint32_t>(Result);
}

template <int kFraction = kFractionWidth>
float FromFixedPoint(uint32_t Value)
{
    float Result = static_cast<float>(static_cast<int32_t>(Value)) / pow(2, kFraction);

    return Result;
}