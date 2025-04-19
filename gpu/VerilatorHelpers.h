#pragma once

#include <verilated.h>

#include "gpu/Matrix44.h"

void HelperSetFixedPointVector(VlWide<4UL>& FixedPointVector, const FVector4& Vector);

FVector4 HelperGetFixedPointVector(const VlWide<4UL>& FixedPointVector);

void HelperSetFixedPointMatrix(VlWide<16UL>& FixedPointMatrix, const FMatrix44& Matrix);