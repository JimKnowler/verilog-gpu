#pragma once

// Verilator
#include <verilated.h>

#include "gpu/Matrix.h"

void HelperExpectEq(const FVector4& Expected, const FVector4& Actual);

void HelperSetFixedPointVector(VlWide<4UL>& FixedPointVector, const FVector4& Vector);

FVector4 HelperGetFixedPointVector(const VlWide<4UL>& FixedPointVector);

void HelperSetFixedPointMatrix(VlWide<16UL>& FixedPointMatrix, const FMatrix44& Matrix);
