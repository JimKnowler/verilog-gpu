#pragma once

// WARNING - if this header is included in multiple compile units, then it will
//           redefine multiple symbols, causing link failure.
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Verilator
#include <verilated.h>

// Verilog Modules
#include "VTriangleAssembly.h"
#include "VTriangleRasterizer.h"

// Gpu
#include "Matrix44.h"
#include "Model.h"

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
    
    int GetRenderBufferPixelIndex(int x, int y) const;
    std::vector<olc::Pixel>& GetBackBuffer();
    std::vector<olc::Pixel>& GetFrontBuffer();
    void SwapRenderBuffers();
    void ClearBackBuffer();

    void InitRasterizer();
    void StepRasterizer();

    void InitTriangleAssembly();
    void StartTriangleAssembly();
    void StepTriangleAssembly();

    FMatrix44 MakeWorldTransform() const;
    FMatrix44 MakeViewProjectionTransform() const;

    void InitModel();
    void InitMemory();

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
    VTriangleAssembly TriangleAssembly;

    std::vector<uint32_t> Memory;

    ///////////////////////////////////
    // Animation
    
    float Rotation = 0.0f;

    ///////////////////////////////////
    // Model Description

    FModel Model;
    
};
