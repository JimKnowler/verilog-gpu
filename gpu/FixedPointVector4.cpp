#include "FixedPointVector4.h"

FFixedPointVector4::FFixedPointVector4() : X(0), Y(0), Z(0), W(0)
{
}

FFixedPointVector4::FFixedPointVector4(uint32_t _X, uint32_t _Y, uint32_t _Z, uint32_t _W)
: X(_X), Y(_Y), Z(_Z), W(_W)
{
}

FFixedPointVector4::FFixedPointVector4(float _X, float _Y, float _Z, float _W)
: X(ToFixedPoint(_X)), Y(ToFixedPoint(_Y)), Z(ToFixedPoint(_Z)), W(ToFixedPoint(_W))
{

}

FFixedPointVector4 &FFixedPointVector4::operator=(const FVector4 &Other)
{
    X = ToFixedPoint(Other.X);
    Y = ToFixedPoint(Other.Y);
    Z = ToFixedPoint(Other.Z);
    W = ToFixedPoint(Other.W);

    return *this;
}

FFixedPointVector4::FFixedPointVector4(const FVector4 &Other)
{
    *this = Other;
}
