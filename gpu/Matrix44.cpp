#include "Matrix44.h"

#include <math.h>
#include <assert.h>

namespace 
{
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

FMatrix44 FMatrix44::operator*(const FMatrix44 &Other) const
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

FVector4 FMatrix44::operator*(const FVector4 &Vector) const
{
    FVector4 Result;

    for (int i=0; i<4; i++) 
    {
        Result[i] = Rows[i].Dot(Vector);
    }
    
    return Result;
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

FMatrix44 FMatrix44::RotateZ(float Radians)
{
    FMatrix44 Matrix = Identity();
    Matrix.Rows[0].X = cosf(Radians);
    Matrix.Rows[0].Y = -sinf(Radians);
    Matrix.Rows[1].X = sinf(Radians);
    Matrix.Rows[1].Y = cosf(Radians);

    return Matrix;
}

FMatrix44 FMatrix44::RotateY(float Radians)
{
    FMatrix44 Matrix = Identity();
    Matrix.Rows[0].X = cosf(Radians);
    Matrix.Rows[0].Z = sinf(Radians);
    Matrix.Rows[2].X = -sinf(Radians);
    Matrix.Rows[2].Z = cosf(Radians);

    return Matrix;
}

FMatrix44 FMatrix44::RotateX(float Radians)
{
    FMatrix44 Matrix = Identity();
    Matrix.Rows[1].Y = cosf(Radians);
    Matrix.Rows[1].Z = -sinf(Radians);
    Matrix.Rows[2].Y = sinf(Radians);
    Matrix.Rows[2].Z = cosf(Radians);

    return Matrix;
}

FMatrix44 FMatrix44::LookAt(const FVector4& Eye, const FVector4& Center, const FVector4& Up)
{
    assert(Eye.IsPoint());
    assert(Center.IsPoint());
    assert(Up.IsDirection());

    const FVector4 ZAxis = (Eye - Center).Normalise();                  // Backwards
    const FVector4 XAxis = (Up.CrossProduct(ZAxis)).Normalise();        // Right
    const FVector4 YAxis = ZAxis.CrossProduct(XAxis);                   // Up

    // Rotation
    FMatrix44 View(
        XAxis,
        YAxis,
        ZAxis,
        FVector4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    // Translation
    View[0].W = -(XAxis.Dot(Eye));
    View[1].W = -(YAxis.Dot(Eye));
    View[2].W = -(ZAxis.Dot(Eye));

    return View;
}

FMatrix44 FMatrix44::Perspective(float FOV, float Aspect, float Near, float Far)
{
    const float HalfFOV = FOV * 0.5f;
    const float TanHalfFOV = tanf(HalfFOV);
    const float FarPlusNear = Far + Near;
    const float FarMinusNear = Far - Near;

    FMatrix44 M = FMatrix44::Zero();
    M[0][0] = 1.0f / (Aspect * TanHalfFOV);
    M[1][1] = 1.0f / TanHalfFOV;
    M[2][2] = -(FarPlusNear / FarMinusNear);
    M[2][3] = -(2.0f * Far * Near) / FarMinusNear;
    M[3][2] = -1.0f;

    return M;
}
