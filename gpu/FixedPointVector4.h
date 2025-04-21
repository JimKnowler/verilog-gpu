#pragma once

#include "FixedPoint.h"

struct FFixedPointVector4
{
    uint32_t X, Y, Z, W;

    FFixedPointVector4(uint32_t _X, uint32_t _Y, uint32_t _Z, uint32_t _W)
     : X(_X), Y(_Y), Z(_Z), W(_W)
    {
    }
    
    FFixedPointVector4(float _X, float _Y, float _Z, float _W)
     : X(ToFixedPoint(_X)), Y(ToFixedPoint(_Y)), Z(ToFixedPoint(_Z)), W(ToFixedPoint(_W))
    {

    }
};
