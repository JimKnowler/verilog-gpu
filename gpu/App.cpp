#include "App.h"
#include "FixedPoint.h"
#include "Matrix44.h"

namespace {
    const uint32_t kScreenWidth = 1280;
    const uint32_t kScreenHeight = 720;

    const olc::vi2d kOrigin = {400, 150};
    const olc::vi2d kRasterSize = {400, 400};

    const int kRasterizerBatchSize = 10000;

    // vertex colours
    olc::Pixel c1{255, 0, 0};
    olc::Pixel c2{0, 255, 0};
    olc::Pixel c3{0, 0, 255};
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

    // Vertex colours
    Rasterizer->i_v1r = ToFixedPoint(c1.r / 255.0f);
    Rasterizer->i_v1g = ToFixedPoint(c1.g / 255.0f);
    Rasterizer->i_v1b = ToFixedPoint(c1.b / 255.0f);
    
    Rasterizer->i_v2r = ToFixedPoint(c2.r / 255.0f);
    Rasterizer->i_v2g = ToFixedPoint(c2.g / 255.0f);
    Rasterizer->i_v2b = ToFixedPoint(c3.b / 255.0f);

    Rasterizer->i_v3r = ToFixedPoint(c3.r / 255.0f);
    Rasterizer->i_v3g = ToFixedPoint(c3.g / 255.0f);
    Rasterizer->i_v3b = ToFixedPoint(c3.b / 255.0f);

    InitRotateTriangle();

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

            TickRotateTriangle();
        }
    }
}

void App::RasterizePixel(int x, int y)
{
    Rasterizer->i_x = x;
    Rasterizer->i_y = y;

    Rasterizer->eval();

    const int r = Rasterizer->o_r;
    const int g = Rasterizer->o_g;
    const int b = Rasterizer->o_b;

    const int Index = GetRenderBufferIndex(x, y);
    olc::Pixel& Pixel = RenderBuffer[Index];
    Pixel = olc::Pixel(r, g, b);
}

int App::GetRenderBufferIndex(int x, int y) const
{
    return (y * kRasterSize.x) + x;
}

void App::InitRotateTriangle()
{
    Rotation = 0.0f;

    TickRotateTriangle();
}

void App::TickRotateTriangle()
{
    const float DeltaTime = 0.1f;
    const float RotationSpeed = M_PI;

    Rotation += (DeltaTime * RotationSpeed);
    Rotation = fmodf(Rotation, 2.0f * M_PI);

    FVector4 v1(50, 40, 0, 1);
    FVector4 v2(300, 300, 0, 1);
    FVector4 v3(100, 360, 0, 1);

    const FVector4 Center(kRasterSize.x / 2, kRasterSize.y / 2, 0);
    FMatrix44 Rotate = FMatrix44::Translation(Center.X, Center.Y, 0) * FMatrix44::RotateZ(Rotation) * FMatrix44::Translation(-Center.X, -Center.Y, 0);
    v1 = Rotate * v1;
    v2 = Rotate * v2;
    v3 = Rotate * v3;

    Rasterizer->i_v1x = ToFixedPoint(v1.X);
    Rasterizer->i_v1y = ToFixedPoint(v1.Y);

    Rasterizer->i_v2x = ToFixedPoint(v2.X);
    Rasterizer->i_v2y = ToFixedPoint(v2.Y);

    Rasterizer->i_v3x = ToFixedPoint(v3.X);
    Rasterizer->i_v3y = ToFixedPoint(v3.Y); 
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
