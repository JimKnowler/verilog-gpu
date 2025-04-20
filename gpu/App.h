#pragma once

// WARNING - if this header is included in multiple compile units, then it will
//           redefine multiple symbols, causing link failure.
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Verilator
#include <verilated.h>

// Verilog Modules
#include "VTriangleRasterizer.h"
#include "VVertexTransform.h"

#include "Matrix44.h"

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
    void RenderFrame(const olc::vi2d& Origin, const std::vector<olc::Pixel>& RenderBuffer);
    void RasterizePixel();
    int GetRenderBufferIndex(int x, int y) const;
    
    void InitRotateTriangle();
    void TickRotateTriangle();
    void SwapRenderBuffers();

    std::vector<olc::Pixel>& GetBackBuffer();
    std::vector<olc::Pixel>& GetFrontBuffer();

    void ClearBackBuffer();

    void StartRenderingTriangle();
    void ResetRasterizer();
    void StepRasterizer();

    FMatrix44 MakeModelViewProjectionTransform() const;

    enum
    {
        kNumRenderBuffers = 2
    };

    int FrontBuffer = 0;
    std::vector<olc::Pixel> RenderBuffers[kNumRenderBuffers];

    VTriangleRasterizer Rasterizer;
    VVertexTransform VertexTransform;

    float Rotation = 0.0f;
};
