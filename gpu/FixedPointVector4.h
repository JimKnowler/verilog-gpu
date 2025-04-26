#pragma once

#include "FixedPoint.h"
#include "Vector4.h"

struct FFixedPointVector4
{
    uint32_t X, Y, Z, W;

    FFixedPointVector4();

    FFixedPointVector4(uint32_t _X, uint32_t _Y, uint32_t _Z, uint32_t _W);
    
    FFixedPointVector4(float _X, float _Y, float _Z, float _W);

    FFixedPointVector4& operator=(const FVector4& Other);

    FFixedPointVector4(const FVector4& Other);

    FVector4 ToVector4() const;
};
