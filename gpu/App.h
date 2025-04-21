#pragma once

// WARNING - if this header is included in multiple compile units, then it will
//           redefine multiple symbols, causing link failure.
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Verilator
#include <verilated.h>

// Verilog Modules
#include "VTriangleRasterizer.h"
#include "VTriangleBackFaceCull.h"
#include "VVertexTransform.h"

// Math
#include "Matrix44.h"
#include "Vertex.h"

class App : public olc::PixelGameEngine
{
public:
    App();

    /// @brief called once at start
    /// @return true, if successful
    bool OnUserCreate() override;

    /// @brief Called once at end
    bool OnUserDestroy() override;

    // @brief called every frame
    /// @return true, if successful
    bool OnUserUpdate(float fElapsedTime) override;

private:
    void Update(float DeltaTime);
    void Render();
    
    void DrawRenderBuffer(const olc::vi2d& Origin, const std::vector<olc::Pixel>& RenderBuffer);
    void RenderPixelFromRasterizerToBackBuffer();
    
    void InitAnimation();
    void TickAnimation();
    
    int GetRenderBufferIndexForPixel(int x, int y) const;
    std::vector<olc::Pixel>& GetBackBuffer();
    std::vector<olc::Pixel>& GetFrontBuffer();
    void SwapRenderBuffers();

    void ClearBackBuffer();

    void StartRenderingModel();
    bool StartRenderingNextTriangle();
    void RenderTriangle(int Index);

    void InitRasterizer();
    void StepRasterizer();

    FMatrix44 MakeWorldTransform() const;
    FMatrix44 MakeViewProjectionTransform() const;

    FVector4 ApplyProjectionTransform(const FMatrix44& Transform, const FVector4 Vertex);

    void InitModel();

    ///////////////////////////////////
    // Render Buffers
    
    enum
    {
        kNumRenderBuffers = 2
    };

    int FrontBuffer = 0;
    std::vector<olc::Pixel> RenderBuffers[kNumRenderBuffers];

    ///////////////////////////////////
    // Verilog Render pipeline
    
    VTriangleRasterizer Rasterizer;
    VVertexTransform VertexTransform;
    VTriangleBackFaceCull BackFaceCull;

    std::vector<VlWide<4UL>*> VertexInputPorts;
    std::vector<VlWide<4UL>*> BackFaceCullInputPorts;
    std::vector<VlWide<4UL>*> ColourInputPorts;

    ///////////////////////////////////
    // Animation
    
    float Rotation = 0.0f;

    ///////////////////////////////////
    // Model Description

    int NumTriangles = 0;
    
    std::vector<FVertex> VertexBuffer;
    std::vector<uint32_t> IndexBuffer;

    ///////////////////////////////////
    // Model Rendering

    int TriangleIndex = 0;
};
