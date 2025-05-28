#pragma once

// WARNING - if this header is included in multiple compile units, then it will
//           redefine multiple symbols, causing link failure.
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Rasterizers : public olc::PixelGameEngine
{
public:
    Rasterizers();

    /// @brief called once at start
    /// @return true, if successful
    bool OnUserCreate() override;

    /// @brief Called once at end
    bool OnUserDestroy() override;

    // @brief called every frame
    /// @return true, if successful
    bool OnUserUpdate(float fElapsedTime) override;

private:
    struct FTriangle {
        olc::vi2d v1;
        olc::vi2d v2;
        olc::vi2d v3;
    };

    void Update(float DeltaTime);
    void Render();
    
    void DrawRenderBuffer(const olc::vi2d& Origin, const std::vector<olc::Pixel>& RenderBuffer);
    int GetRenderBufferPixelIndex(int x, int y) const;

    void RasterizeBoundingBox(const FTriangle& Triangle, std::vector<olc::Pixel>& RenderBuffer);
    void RasterizeZigZag(const FTriangle& Triangle, std::vector<olc::Pixel>& RenderBuffer);

    void GetStartingVertex(const std::vector<olc::vi2d>& vertices, olc::vi2d& v);
};
