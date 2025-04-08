#include "App.h"
#include "FixedPoint.h"

namespace {
    const uint32_t kScreenWidth = 1280;
    const uint32_t kScreenHeight = 720;

    const olc::vi2d kOrigin = {400, 150};
    const olc::vi2d kRasterSize = {400, 400};

    const int kRasterizerBatchSize = 2000;

    olc::vi2d v1{50, 40};
    olc::vi2d v2{300, 300};
    olc::vi2d v3{100, 360};
}

int main(int argc, char* argv[])
{
    printf("hello gpu\n");

    App app;

    if (app.Construct(kScreenWidth, kScreenHeight, 1, 1))
    {
        app.Start();
    }

    return 0;
}

App::App() 
{
    sAppName = "verilog-gpu";
}

bool App::OnUserCreate() 
{
    printf("OnUserCreate\n");

    RenderBuffer.resize(kRasterSize.x * kRasterSize.y, olc::WHITE);

    x = 0;
    y = 0;

    // Construct a VerilatedContext to hold simulation time, etc.
    Context = new VerilatedContext;

    // Construct the Verilated model, from VTriangleRasterizer.h generated from Verilating "VTriangleRasterizer.v"
    Rasterizer = new VTriangleRasterizer{Context};

    Rasterizer->v1x = ToFixedPoint(v1.x);
    Rasterizer->v1y = ToFixedPoint(v1.y);
    Rasterizer->v2x = ToFixedPoint(v2.x);
    Rasterizer->v2y = ToFixedPoint(v2.y);
    Rasterizer->v3x = ToFixedPoint(v3.x);
    Rasterizer->v3y = ToFixedPoint(v3.y);    

    return true;
}

bool App::OnUserDestroy()
{
    // Final verilatedmodel cleanup
    Rasterizer->final();

    // Destroy model
    delete Rasterizer;
    delete Context;

    return true;
}

bool App::OnUserUpdate(float fElapsedTime) {
    Update(fElapsedTime);

    Render();

    return true;
}

void App::Update(float DeltaTime)
{
    if (GetKey(olc::ESCAPE).bReleased) {
        exit(0);
    }

    // rasterize some pixels
    for (int n = 0; n < kRasterizerBatchSize; n++) 
    {
        RasterizePixel(x, y);

        x += 1;
        if (x >= kRasterSize.x) {
            x = 0;
            y += 1;
        }

        if (y >= kRasterSize.y) {
            y = 0;
        }
    }
}

void App::RasterizePixel(int x, int y)
{
    Rasterizer->x = ToFixedPoint(x);
    Rasterizer->y = ToFixedPoint(y);

    Rasterizer->eval();

    const int r = Rasterizer->r;
    const int g = Rasterizer->g;
    const int b = Rasterizer->b;

    const int Index = GetRenderBufferIndex(x, y);
    olc::Pixel& Pixel = RenderBuffer[Index];
    Pixel = olc::Pixel(r, g, b);
}

int App::GetRenderBufferIndex(int x, int y) const
{
    return (y * kRasterSize.x) + x;
}

void App::Render()
{
    FillRect({ 0,0 }, { ScreenWidth(), ScreenHeight() }, olc::GREY);

#if 0
    DrawRect(kOrigin, kRasterSize, olc::WHITE);
#else
    for (int x=0; x<kRasterSize.x; x++)
    {
        for (int y=0; y<kRasterSize.y; y++)
        {
            const int Index = GetRenderBufferIndex(x, y);
            const olc::Pixel Pixel = RenderBuffer[Index];

            DrawRect(kOrigin + olc::vi2d{x,y}, {1,1}, Pixel);
        }
    }
#endif
}
