#pragma once

/**
 * @brief 4d Vector
 * 
 */
struct FVector4
{
    union {
        struct {
            float X, Y, Z, W;
        };
        float Values[4];
    };

    FVector4();

    FVector4(float _X, float _Y, float _Z, float _W);

    static const FVector4& Zero();

    float Dot(const FVector4& Other) const;

    FVector4 operator+(const FVector4& Other) const;

    FVector4 operator*(float Scalar) const;

    FVector4 operator/(float Scalar) const;

    float& operator[](int Index);

    float operator[](int Index) const;
};
