#pragma once

#include "gpu/Matrix44.h"

void HelperExpectEq(const float Expected, const float Actual);

void HelperExpectEq(const FVector4& Expected, const FVector4& Actual);

void HelperExpectEq(const FMatrix44& Expected, const FMatrix44& Actual);

float DegreesToRadians(float Degrees);