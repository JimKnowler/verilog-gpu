#include "Matrix.h"

namespace {

    static const FVector4 kZeroVector4(
        0.0f, 0.0f, 0.0f, 0.0f
    );

    static const FMatrix44 kZero(
        FVector4::Zero(), 
        FVector4::Zero(), 
        FVector4::Zero(), 
        FVector4::Zero()
    );

    static const FMatrix44 kIdentity(
        FVector4(1.0f, 0.0f, 0.0f, 0.0f),
        FVector4(0.0f, 1.0f, 0.0f, 0.0f),
        FVector4(0.0f, 0.0f, 1.0f, 0.0f),
        FVector4(0.0f, 0.0f, 0.0f, 1.0f)
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

FMatrix44::FMatrix44(const FVector4& Row0, const FVector4& Row1, const FVector4& Row2, const FVector4& Row3)
{
    Rows[0] = Row0;
    Rows[1] = Row1;
    Rows[2] = Row2;
    Rows[3] = Row3;
}

FMatrix44& FMatrix44::operator=(const FMatrix44& Other)
{
    for (int i=0; i<4; i++) {
        Rows[i] = Other.Rows[i];
    }

    return *this;
}

const FMatrix44& FMatrix44::Zero()
{
    return kZero;
}

const FMatrix44& FMatrix44::Identity()
{
    return kIdentity;
}
