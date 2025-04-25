#pragma once

#include <vector>

#include "FixedPointVertex.h"

struct FModel
{
    int NumTriangles = 0;
    
    std::vector<FFixedPointVertex> VertexBuffer;
    
    std::vector<uint32_t> IndexBuffer;

    void InitCube(float Radius);
};
