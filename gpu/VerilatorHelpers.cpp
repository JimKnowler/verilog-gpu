#include "VerilatorHelpers.h"
#include "FixedPoint.h"

void HelperSetFixedPointVector(VlWide<4UL>& FixedPointVector, const FVector4& Vector)
{        
    FixedPointVector.at(0) = ToFixedPoint(Vector.X);
    FixedPointVector.at(1) = ToFixedPoint(Vector.Y);
    FixedPointVector.at(2) = ToFixedPoint(Vector.Z);
    FixedPointVector.at(3) = ToFixedPoint(Vector.W);
}

FVector4 HelperGetFixedPointVector(const VlWide<4UL>& FixedPointVector)
{
    FVector4 Result;
    Result.X = FromFixedPoint(FixedPointVector.at(0));
    Result.Y = FromFixedPoint(FixedPointVector.at(1));
    Result.Z = FromFixedPoint(FixedPointVector.at(2));
    Result.W = FromFixedPoint(FixedPointVector.at(3));

    return Result;
}

void HelperSetFixedPointMatrix(VlWide<16UL>& FixedPointMatrix, const FMatrix44& Matrix)
{        
    for (int i=0; i<4; i++) 
    {
        const FVector4& Row = Matrix.Rows[i];
        const int Offset = i * 4;
        
        FixedPointMatrix.at(Offset + 0) = ToFixedPoint(Row.X);
        FixedPointMatrix.at(Offset + 1) = ToFixedPoint(Row.Y);
        FixedPointMatrix.at(Offset + 2) = ToFixedPoint(Row.Z);
        FixedPointMatrix.at(Offset + 3) = ToFixedPoint(Row.W);
    }
}