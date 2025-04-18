#include "Vector4.h"

#include <math.h>

namespace 
{
    static const FVector4 kZeroVector4(
        0.0f, 0.0f, 0.0f, 0.0f
    );
}

FVector4::FVector4()
{

}

FVector4::FVector4(float _X, float _Y, float _Z, float _W)
    : X(_X), Y(_Y), Z(_Z), W(_W)
{

}

const FVector4& FVector4::Zero()
{
    return kZeroVector4;
}

float FVector4::Dot(const FVector4 &Other) const
{
    return (X * Other.X) + (Y * Other.Y) + (Z * Other.Z) + (W * Other.W);
}

FVector4 FVector4::operator+(const FVector4 &Other) const
{
    return FVector4(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
}

FVector4 FVector4::operator*(float Scalar) const
{
    return FVector4(X * Scalar, Y * Scalar, Z * Scalar, W * Scalar);
}

FVector4 FVector4::operator/(float Scalar) const
{
    return FVector4(X / Scalar, Y / Scalar, Z / Scalar, W / Scalar);
}

float &FVector4::operator[](int Index)
{
    return Values[Index];
}

float FVector4::operator[](int Index) const
{
    return Values[Index];
}