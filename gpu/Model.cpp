#include "Model.h"

#include <assert.h>

namespace
{
    const std::vector<FVector4> kCubeVertices = {
        // Front face (+Z)
        {-1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},

        // Back face (-Z)
        { 1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f},

        // Left face (-X)
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f, -1.0f},

        // Right face (+X)
        { 1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f,  1.0f},

        // Top face (+Y)
        {-1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},

        // Bottom face (-Y)
        {-1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},
    };

    const std::vector<FVector4> kCubeFaceNormals = {
        {0.0f, 0.0f, 1.0f},         // Front +z
        {0.0f, 0.0f, -1.0f},        // Back -z
        {-1.0f, 0.0f, 0.0f},        // Left -x
        {1.0f, 0.0f, 0.0f},         // Right +x
        {0.0f, 1.0f, 0.0f},         // Top +y
        {0.0f, -1.0f, 0.0f},        // Bottom -y
    };

    const FVector4 Red(1.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 Green(0.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 Blue(0.0f, 0.0f, 1.0f, 1.0f);

    const FVector4 kCubeFaceColours[6] = {
        Red,
        Green,
        Blue
    };
}

void FModel::InitCube(float Radius)
{
    const int NumVertices = kCubeVertices.size();

    for (int i=0; i<NumVertices; i++)
    {
        const int Face = i / 4;
        const FVector4 Colour = kCubeFaceColours[Face / 2];
        
        FVector4 Position = kCubeVertices[i] * Radius;
        Position.W = 1.0f;
        assert(Position.IsPoint());

        const FVector4 Normal = kCubeFaceNormals[Face];
        assert(Normal.IsDirection());        
        
        const FFixedPointVertex Vertex{
            .Position = FFixedPointVector4(Position),
            .Normal = FFixedPointVector4(Normal),
            .Colour = FFixedPointVector4(Colour)
        };

        VertexBuffer.push_back(Vertex);
    }

    NumTriangles = 12;

    IndexBuffer = {
        // Front face
        0,  1,  2,
        0,  2,  3,

        // Back face
        4,  5,  6,
        4,  6,  7,

        // Left face
        8,  9, 10,
        8, 10, 11,

        // Right face
        12, 13, 14,
        12, 14, 15,

        // Top face
        16, 17, 18,
        16, 18, 19,

        // Bottom face
        20, 21, 22,
        20, 22, 23,
    };
}
