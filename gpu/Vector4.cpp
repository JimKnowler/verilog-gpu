#include "Vector4.h"

#include <math.h>
#include <assert.h>

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

FVector4 FVector4::CrossProduct(const FVector4 &Other) const
{
    assert(IsDirection());
    assert(Other.IsDirection());

    return FVector4(
        (Y*Other.Z) - (Z*Other.Y),
        (Z*Other.X) - (X*Other.Z),
        (X*Other.Y) - (Y*Other.X),
        0.0f
    );
}

FVector4 FVector4::operator+(const FVector4 &Other) const
{
    return FVector4(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
}

FVector4 FVector4::operator-(const FVector4 &Other) const
{
    return FVector4(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
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

float FVector4::Length() const
{
    return sqrtf(LengthSq());
}

float FVector4::LengthSq() const
{
    return this->Dot(*this);
}

bool FVector4::IsNormalised() const
{
    return fabs(LengthSq() - 1.0f) < 0.0001f;
}

FVector4 FVector4::Normalise() const
{
    return *this / Length();
}

bool FVector4::IsDirection() const
{
    return fabs(W) < 0.0001f;
}

bool FVector4::IsPoint() const
{
    return fabs(W) > 0.0001f;
}

std::string FVector4::ToString() const
{
    char Buffer[64];
    snprintf(Buffer, sizeof(Buffer), 
        "x[%.2f] y[%.2f] z[%.2f] w[%.2f]", 
        X, Y, Z, W
    );

    std::string Str = Buffer;

    return Str;
}
