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

/**
 * @brief Row-Major storage of 4x4 matrix
 * 
 */
struct FMatrix44 
{
    FVector4 Rows[4];

    /**
     * @brief Default constructor
     * 
     * @note All member values are uninitialized
     */
    FMatrix44();

    /**
     * @brief Construct a new FMatrix44 object from its rows
     * 
     * @param Row0 
     * @param Row1 
     * @param Row2 
     * @param Row3 
     */
    FMatrix44(const FVector4& Row0, const FVector4& Row1, const FVector4& Row2, const FVector4& Row3);

    /**
     * @brief Assignment operator
     * 
     * @param Other The other matrix to copy from
     * @return This matrix
     */
    FMatrix44& operator=(const FMatrix44& Other);

    /**
     * @brief Multiply Matrices togther
     * 
     * @param Other The other matrix to multiply with
     * @return FMatrix44 
     */
    FMatrix44 operator*(const FMatrix44& Other);

    /**
     * @brief Retrieve a column from the matrix
     * 
     * @param Index The index of the column 
     * @return FVector4 
     */
    FVector4 Column(int Index);

    /**
     * @brief Get a matrix full of zeros
     * 
     * @return const FMatrix44& 
     */
    static const FMatrix44& Zero();

    /**
     * @brief Get an identity matrix
     * 
     * @return const FMatrix44& 
     */
    static const FMatrix44& Identity();

    /**
     * @brief Create a translation matrix
     * 
     * @return Translation Matrix
     */
    static FMatrix44 Translation(float X, float Y, float Z);

    /**
     * @brief Create a scale matrix
     * 
     * @param ScaleX
     * @param ScaleY
     * @param ScaleZ
     * 
     * @return FMatrix44 
     */
    static FMatrix44 Scale(float ScaleX, float ScaleY, float ScaleZ);

    /**
     * @brief Create a uniform scale matrix
     * 
     * @param Scale 
     * @return FMatrix44 
     */
    static FMatrix44 Scale(float Scale);

    /**
     * @brief Create a yaw rotation matrix
     * 
     * @param Radians Rotation angle in radians
     * @return FMatrix44 
     */
    static FMatrix44 RotateYaw(float Radians);

    /**
     * @brief Create a pitch rotation matrix
     * 
     * @param Radians Rotation angle in radians
     * @return FMatrix44 
     */
    static FMatrix44 RotatePitch(float Radians);

    /**
     * @brief Create a roll rotation matrix
     * 
     * @param Radians Rotation angle in radians
     * @return FMatrix44 
     */
    static FMatrix44 RotateRoll(float Radians);
};

