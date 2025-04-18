#pragma once

#include "Vector4.h"

/**
 * @brief 4x4 matrix
 * 
 * @note Uses Row-Major Order storage
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
     * @return Concatenated Matrix
     */
    FMatrix44 operator*(const FMatrix44& Other) const;

    /**
     * @brief Multiply Vector by this Matrix
     * 
     * @param Vector The Vector to multiply
     * @return Vector transformed by Matrix
     */
    FVector4 operator*(const FVector4& Vector) const;

    /**
     * @brief Access a Row in the matrix
     * 
     * @param Index of the row to access
     * @return FVector4 of the requested Row
     */
    FVector4& operator[](int Index);

    /**
     * @brief Access a Row in the matrix
     * 
     * @param Index of the row to access
     * @return FVector4 of the requested Row
     */
    const FVector4& operator[](int Index) const;

    /**
     * @brief Retrieve a column from the matrix
     * 
     * @param Index The index of the column 
     * @return FVector4 
     */
    FVector4 Column(int Index) const;

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
     * @brief Create a Z rotation matrix
     * 
     * @note Rotates clockwise around the axis
     * @note Uses right-hand-rule axes (Negative Z-axis pointing into scene)
     * 
     * @param Radians Rotation angle in radians
     * @return FMatrix44 
     */
    static FMatrix44 RotateZ(float Radians);

    /**
     * @brief Create a Y rotation matrix
     * 
     * @note Rotates clockwise around the axis
     * @note Uses right-hand-rule axes (Negative Z-axis pointing into scene)
     * 
     * @param Radians Rotation angle in radians
     * @return FMatrix44 
     */
    static FMatrix44 RotateY(float Radians);

    /**
     * @brief Create a Z rotation matrix
     * 
     * @note Rotates clockwise around the axis
     * @note Uses right-hand-rule axes (Negative Z-axis pointing into scene)
     * 
     * @param Radians Rotation angle in radians
     * @return FMatrix44 
     */
    static FMatrix44 RotateX(float Radians);


    /**
     * @brief Create a View Transform matrix
     * 
     * @note Uses right-hand-rule axes (Negative Z-axis pointing into scene)
     * 
     * @param Eye Location of the Camera
     * @param Center Location of the target point (what we're looking at)
     * @param Up The camera's up direction
     * 
     * @return Matrix that transforms points from world space to view space
     */
    static FMatrix44 LookAt(const FVector4& Eye, const FVector4& Center, const FVector4& Up);


    /**
     * @brief Create a perspective projection matrix
     * 
     * This transforms view-space into clipping space
     * - a unit cube, from +1 to -1 along each axis
     * - NDC: normalised device co-ordinates
     * - Maps depth into range [-1, 1]
     * 
     * @note Uses right-hand-rule axes (Negative Z-axis pointing into scene)
     * 
     * @param FOV Field of view in the y axis (radians)
     * @param Aspect Aspect Ration (width / height)
     * @param Near Distance to the near clipping plane
     * @param Far Distance to the far clipping plane
     */
    static FMatrix44 Perspective(float FOV, float Aspect, float Near, float Far);
};

