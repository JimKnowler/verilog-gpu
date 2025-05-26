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
    auto EdgeFunction = [](const olc::vi2d& v1, const olc::vi2d& v2, int& a, int& b, int& c) -> void {
        a = v1.y - v2.y;
        b = v2.x - v1.x;
        c = (v1.x * v2.y) - (v2.x * v1.y);
    };

    int a1, b1, c1, a2, b2, c2, a3, b3, c3;
    EdgeFunction(Triangle.v2, Triangle.v3, a1, b1, c1);
    EdgeFunction(Triangle.v3, Triangle.v1, a2, b2, c2);
    EdgeFunction(Triangle.v1, Triangle.v2, a3, b3, c3);

    int e1 = c1;
    int e2 = c2;
    int e3 = c3;

    int x = 0;
    int y = 0;
    bool bIsMovingRight = true;
    int dx = 1;

    while (true)
    {
        // rasterize pixel
        const int PixelIndex = GetRenderBufferPixelIndex(x, y);
        const bool bIsInside = (e1 >=0) && (e2 >= 0) && (e3 >= 0);
        RenderBuffer[PixelIndex] = bIsInside ? olc::RED : olc::BLACK;

        // plan next move: left, right, or down
        const int NextX = x + dx;

        if ((NextX >= 0) && (NextX < kRasterSize.x)) {
            x = NextX;

            e1 += a1;
            e2 += a2;
            e3 += a3;
        } else {
            // move vertically down one row
            y += 1;
            if (y >= kRasterSize.y)
            {
                return;
            }
            
            e1 += b1;
            e2 += b2;
            e3 += b3;

            // alternate horizontal direction
            bIsMovingRight = !bIsMovingRight;
            
            a1 = -a1;
            a2 = -a2;
            a3 = -a3;

            dx = -dx;
        }
    }
}

void Rasterizers::RasterizeZigZag(const FTriangle& Triangle, std::vector<olc::Pixel>& RenderBuffer)
{

}