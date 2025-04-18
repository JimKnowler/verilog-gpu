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

    FVector4(float _X, float _Y, float _Z, float _W = 0.0f);

    static const FVector4& Zero();

    float Dot(const FVector4& Other) const;

    /**
     * 3D Cross Product
     * 
     * @note Assumes that W=0 on both this and Other vector
     *       (i.e. that they are directions)
     * @note Always sets W on result to 0
     * 
     * @return 3D cross product of this vector and other. 
     */
    FVector4 CrossProduct(const FVector4& Other) const;

    FVector4 operator+(const FVector4& Other) const;

    FVector4 operator-(const FVector4& Other) const;

    FVector4 operator*(float Scalar) const;

    FVector4 operator/(float Scalar) const;

    float& operator[](int Index);

    float operator[](int Index) const;

    float Length() const;

    float LengthSq() const;

    bool IsNormalised() const;

    FVector4 Normalise() const;

    /**
     * Vectors representing directions have W == 0
     */
    bool IsDirection() const;

    /** 
     * Vectors representing points have W != 0
     */
    bool IsPoint() const;
};
