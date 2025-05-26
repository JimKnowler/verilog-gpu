#include "Rasterizers.h"

namespace {
    const uint32_t kScreenWidth = 1280;
    const uint32_t kScreenHeight = 720;

    const olc::vi2d kOriginBoundingBox = {150, 150};
    const olc::vi2d kOriginZigZag = {650, 150};
    
    const olc::vi2d kRasterSize = {400, 400};    
    const size_t kRasterBufferSize = kRasterSize.x * kRasterSize.y;
}

int main(int argc, char* argv[])
{
    Rasterizers app;

    if (app.Construct(kScreenWidth, kScreenHeight, 1, 1))
    {
        app.Start();
    }

    return 0;
}

Rasterizers::Rasterizers() 
{
    sAppName = "Rasterizers";
}

bool Rasterizers::OnUserCreate() 
{
    printf("OnUserCreate\n");
    
    return true;
}

bool Rasterizers::OnUserDestroy()
{
    printf("OnUserDestroy\n");

    return true;
}

bool Rasterizers::OnUserUpdate(float fElapsedTime) {
    Update(fElapsedTime);

    Render();

    return true;
}

void Rasterizers::Update(float DeltaTime)
{
    if (GetKey(olc::ESCAPE).bReleased) {
        exit(0);
    }    
}

void Rasterizers::Render()
{
    FillRect({ 0,0 }, { ScreenWidth(), ScreenHeight() }, olc::GREY);

    FTriangle Triangle{
        .v1 = { 10, 40 },
        .v2 = { 300, 10 },
        .v3 = { 150, 350 }
    };

    std::vector<olc::Pixel> RenderBufferBoundingBox(kRasterBufferSize);
    RasterizeBoundingBox(Triangle, RenderBufferBoundingBox);
    DrawRenderBuffer(kOriginBoundingBox, RenderBufferBoundingBox);

    std::vector<olc::Pixel> RenderBufferZigZag(kRasterBufferSize);
    RasterizeZigZag(Triangle, RenderBufferBoundingBox);
    DrawRenderBuffer(kOriginZigZag, RenderBufferZigZag);
}

void Rasterizers::DrawRenderBuffer(const olc::vi2d& Origin, const std::vector<olc::Pixel>& RenderBuffer)
{
    for (int x=0; x<kRasterSize.x; x++)
    {
        for (int y=0; y<kRasterSize.y; y++)
        {
            const int Index = GetRenderBufferPixelIndex(x, y);
            const olc::Pixel Pixel = RenderBuffer[Index];

            DrawRect(Origin + olc::vi2d{x,y}, {1,1}, Pixel);
        }
    }
}

int Rasterizers::GetRenderBufferPixelIndex(int x, int y) const
{
    return (y * kRasterSize.x) + x;
}

void Rasterizers::RasterizeBoundingBox(const FTriangle& Triangle, std::vector<olc::Pixel>& RenderBuffer)
{
    auto EdgeFunction = [](int x, int y, const olc::vi2d& v1, const olc::vi2d& v2) -> int {
#if 1
        const int a = v1.y - v2.y;
        const int b = v2.x - v1.x;
        const int c = (v1.x * v2.y) - (v2.x * v1.y);

        const int e = (a * x) + (b * y) + c;
#else
        const int e = ((v2.x - v1.x) * (y - v1.y)) - ((v2.y - v1.y) * (x - v1.x));
#endif

        return e;
    };

    for (int y=0; y<kRasterSize.y; y++)
    {
        for (int x=0; x<kRasterSize.x; x++) 
        {
            const int w1 = EdgeFunction(x, y, Triangle.v2, Triangle.v3);
            const int w2 = EdgeFunction(x, y, Triangle.v3, Triangle.v1);
            const int w3 = EdgeFunction(x, y, Triangle.v1, Triangle.v2);

            const int PixelIndex = GetRenderBufferPixelIndex(x, y);
            const bool bIsInside = (w1 >=0) && (w2 >= 0) && (w3 >= 0);
            RenderBuffer[PixelIndex] = bIsInside ? olc::RED : olc::BLACK;
        }
    }
}

void Rasterizers::RasterizeZigZag(const FTriangle& Triangle, std::vector<olc::Pixel>& RenderBuffer)
{

}