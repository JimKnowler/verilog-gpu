#include "Model.h"

#include <assert.h>
#include <random>

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

void FModel::InitLandscape()
{
    const float Radius = 200.0f;
    const float Height = 100.0f;
    const float PositionOffsetX = -(Radius * 0.5f);
    const float PositionOffsetY = -(Radius * 0.5f);

    const int Resolution = 50;
    const float CellRadius = Radius / float(Resolution);

    const FVector4 kLandscapeColour = Green;

    auto GetIndex = [](int x, int y) -> int {
        return (y * (Resolution+1)) + x;
    };

    const double NoiseOffsetX = 0.0f;
    const double NoiseOffsetY = 0.0f;
    const double NoiseScale = 10.0f;

    // center peak
    for (int y=0; y<=Resolution; y++)
    {
        for (int x=0; x<=Resolution; x++)
        {
            const float siny = sinf(M_PI * float(y) / float(Resolution));
            const float sinx = sinf(M_PI * float(x) / float(Resolution));
            const float strength = 4.0f;
            const float Value = powf(siny, strength) * powf(sinx, strength);
            
            VertexBuffer.push_back(FFixedPointVertex{
                .Position = {PositionOffsetX + (x * CellRadius), PositionOffsetY + (y * CellRadius), Height * Value, 1.0f},
                .Colour = kLandscapeColour
            });
        }
    }    

    // small peak in top left corner
    const int SmallPeakResolution = Resolution / 5;
    const float HeightSmallPeak = Height / 4;
    
    for (int y=0; y<=SmallPeakResolution; y++)
    {
        for (int x=0; x<=SmallPeakResolution; x++)
        {
            const float siny = sinf(M_PI * float(y) / float(SmallPeakResolution));
            const float sinx = sinf(M_PI * float(x) / float(SmallPeakResolution));
            const float strength = 4.0f;
            const float Value = powf(siny, strength) * powf(sinx, strength);
            
            FFixedPointVertex& Vertex = VertexBuffer[GetIndex(x, y)];
            Vertex.Position.Z += ToFixedPoint(Value * HeightSmallPeak);
            Vertex.Colour = (Vertex.Colour.ToVector4() * (1.0f-Value)) + (Red * Value);
        }
    }   

    auto GetNormal = [&](int x, int y) -> FVector4 {    
        const int xa = (x == 0) ? x : x-1;
        const int xb = xa + 1;

        const int ya = (y == 0) ? y : y-1;
        const int yb = ya + 1;

        const FFixedPointVertex v = VertexBuffer[GetIndex(xa, ya)];
        const FFixedPointVertex vx = VertexBuffer[GetIndex(xb, ya)];
        const FFixedPointVertex vy = VertexBuffer[GetIndex(xa, yb)];

        const FVector4 dx = vx.Position.ToVector4() - v.Position.ToVector4();
        const FVector4 dy = vy.Position.ToVector4() - v.Position.ToVector4();

        const FVector4 Normal = dx.CrossProduct(dy).Normalise();

        return Normal;
    };

    for (int y=0; y<=Resolution; y++)
    {
        for (int x=0; x<=Resolution; x++)
        {
            FFixedPointVector4& Normal = VertexBuffer[GetIndex(x,y)].Normal;
            Normal = GetNormal(x, y);
        }
    }

    // generate index buffer

    for (int y=0; y<Resolution; y++)
    {
        for (int x=0; x<Resolution; x++)
        {
            // for each cell
            const uint32_t I00 = GetIndex(x, y);
            const uint32_t I10 = GetIndex(x+1, y);
            const uint32_t I01 = GetIndex(x, y+1);
            const uint32_t I11 = GetIndex(x+1, y+1);

            assert(I11 < VertexBuffer.size());

            IndexBuffer.insert(IndexBuffer.end(), {
                I00, I10, I11,
                I00, I11, I01
            });
        }
    }    

    NumTriangles = (Resolution * Resolution * 2);
}
