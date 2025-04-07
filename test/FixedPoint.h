#pragma once

#include <cinttypes>
#include <math.h>

template <int kFraction = 8>
uint32_t ToFixedPoint(float Value)
{
    uint32_t Result = static_cast<uint32_t>(Value * pow(2, kFraction));

    return Result;
}

template <int kFraction = 8>
float FromFixedPoint(uint32_t Value)
{
    float Result = static_cast<float>(Value) / pow(2, kFraction);

    return Result;
}