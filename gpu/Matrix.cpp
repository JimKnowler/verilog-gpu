#include "Matrix.h"

#include <math.h>
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

FMatrix44::FMatrix44()
{
}

FMatrix44::FMatrix44(const FVector4 &Row0, const FVector4 &Row1, const FVector4 &Row2, const FVector4 &Row3)
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

FMatrix44 FMatrix44::operator*(const FMatrix44 &Other)
{
    FMatrix44 M;
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            M.Rows[i][j] = Rows[i].Dot(Other.Column(j));
        }
    }

    return M;
}

FVector4 &FMatrix44::operator[](int Index)
{
    return Rows[Index];
}

const FVector4 &FMatrix44::operator[](int Index) const
{
    return Rows[Index];
}

FVector4 FMatrix44::Column(int Index) const
{
    return FVector4(Rows[0][Index], Rows[1][Index], Rows[2][Index], Rows[3][Index]);
}

const FMatrix44 &FMatrix44::Zero()
{
    return kZero;
}

const FMatrix44& FMatrix44::Identity()
{
    return kIdentity;
}

FMatrix44 FMatrix44::Translation(float X, float Y, float Z)
{
    FMatrix44 Matrix = Identity();
    Matrix.Rows[0].W = X;
    Matrix.Rows[1].W = Y;
    Matrix.Rows[2].W = Z;

    return Matrix;
}

FMatrix44 FMatrix44::Scale(float ScaleX, float ScaleY, float ScaleZ)
{
    FMatrix44 Matrix = Identity();
    Matrix.Rows[0].X = ScaleX;
    Matrix.Rows[1].Y = ScaleY;
    Matrix.Rows[2].Z = ScaleZ;
    
    return Matrix;
}

FMatrix44 FMatrix44::Scale(float Scale)
{
    FMatrix44 Matrix = Identity();
    Matrix.Rows[0].X = Scale;
    Matrix.Rows[1].Y = Scale;
    Matrix.Rows[2].Z = Scale;

    return Matrix;
}

FMatrix44 FMatrix44::RotateYaw(float Radians)
{
    FMatrix44 Matrix = Identity();
    Matrix.Rows[0].X = cosf(Radians);
    Matrix.Rows[0].Y = -sinf(Radians);
    Matrix.Rows[1].X = sinf(Radians);
    Matrix.Rows[1].Y = cosf(Radians);

    return Matrix;
}

FMatrix44 FMatrix44::RotatePitch(float Radians)
{
    FMatrix44 Matrix = Identity();
    Matrix.Rows[0].X = cosf(Radians);
    Matrix.Rows[0].Z = sinf(Radians);
    Matrix.Rows[1].X = -sinf(Radians);
    Matrix.Rows[1].Z = cosf(Radians);

    return Matrix;
}

FMatrix44 FMatrix44::RotateRoll(float Radians)
{
    FMatrix44 Matrix = Identity();
    Matrix.Rows[1].Y = cosf(Radians);
    Matrix.Rows[1].Z = -sinf(Radians);
    Matrix.Rows[2].Y = sinf(Radians);
    Matrix.Rows[2].Z = cosf(Radians);

    return Matrix;
}
