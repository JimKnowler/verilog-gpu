#pragma once

/**
 * @brief 4d Vector
 * 
 */
struct FVector4
{
    float X, Y, Z, W;

    FVector4();

    FVector4(float _X, float _Y, float _Z, float _W);

    static const FVector4& Zero();
};

/**
 * @brief Row-Major storage of 4x4 matrix
 * 
 */
struct FMatrix44 
{
    FVector4 Rows[4];

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
};

