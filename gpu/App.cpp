#include "App.h"
#include "FixedPoint.h"
#include "VerilatorHelpers.h"

namespace {
    const uint32_t kScreenWidth = 1280;
    const uint32_t kScreenHeight = 720;

    const olc::vi2d kOriginFrontBuffer = {150, 150};
    const olc::vi2d kOriginBackBuffer = {650, 150};
    const olc::vi2d kRasterSize = {400, 400};

    const int kRasterizerBatchSize = 10000;

    const FVector4 c1(1.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 c2(0.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 c3(0.0f, 0.0f, 1.0f, 1.0f);

    float DegreesToRadians(float Degrees)
    {
        float Radians = Degrees * 2.0f * M_PI / 360.0f;

        return Radians;
    }

    const olc::Pixel kClearColour = olc::WHITE;
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

    for (int i=0; i<kNumRenderBuffers; i++)
    {
        RenderBuffers[i].resize(kRasterSize.x * kRasterSize.y, kClearColour);    
    }
    
    HelperSetFixedPointVector(Rasterizer.i_c1, c1);
    HelperSetFixedPointVector(Rasterizer.i_c2, c2);
    HelperSetFixedPointVector(Rasterizer.i_c3, c3);
        
    InitAnimation();
    ResetRasterizer();
    StartRenderingTriangle();

    return true;
}

bool App::OnUserDestroy()
{
    // Final verilatedmodel cleanup
    Rasterizer.final();

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
        if (Rasterizer.o_idle)
        {
            // frame complete
            SwapRenderBuffers();
            ClearBackBuffer();
            TickAnimation();
            StartRenderingTriangle();

            continue;
        }

        if (Rasterizer.o_write)
        {
            RenderPixelFromRasterizerToBackBuffer();
        } 

        StepRasterizer();
    }
}

void App::RenderPixelFromRasterizerToBackBuffer()
{
    int x = Rasterizer.o_x;
    int y = Rasterizer.o_y;

    const int Index = GetRenderBufferIndexForPixel(x, y);
    olc::Pixel& Pixel = GetBackBuffer()[Index];
    
    const FVector4 Colour = HelperGetFixedPointVector(Rasterizer.o_colour);

    const std::function FloatToUint8 = [](float Value) -> uint8_t {
        return static_cast<uint8_t>(255.0f * std::clamp(Value, 0.0f, 1.0f));
    };

    const uint8_t r = FloatToUint8(Colour.X);
    const uint8_t g = FloatToUint8(Colour.Y);
    const uint8_t b = FloatToUint8(Colour.Z);

    Pixel = olc::Pixel(r, g, b);
}

int App::GetRenderBufferIndexForPixel(int x, int y) const
{
    return (y * kRasterSize.x) + x;
}

void App::InitAnimation()
{
    Rotation = 0.0f;
}

void App::TickAnimation()
{
    const float DeltaTime = 0.1f;
    const float RotationSpeed = M_PI;

    Rotation += (DeltaTime * RotationSpeed);
    Rotation = fmodf(Rotation, 2.0f * M_PI);
}

std::vector<olc::Pixel> &App::GetBackBuffer()
{
    assert(FrontBuffer >= 0);
    assert(FrontBuffer <= 1);

    return RenderBuffers[1-FrontBuffer];
}

std::vector<olc::Pixel> &App::GetFrontBuffer()
{
    assert(FrontBuffer >= 0);
    assert(FrontBuffer <= 1);

    return RenderBuffers[FrontBuffer];
}

void App::ClearBackBuffer()
{
    auto& BackBuffer = GetBackBuffer();

    const int kNumPixels = kRasterSize.x * kRasterSize.y;
    for (int i=0; i<kNumPixels; i++)
    {
        BackBuffer[i] = kClearColour;
    }
}

void App::SwapRenderBuffers()
{
    assert(FrontBuffer >= 0);
    assert(FrontBuffer <= 1);

    FrontBuffer = 1 - FrontBuffer;
}

FMatrix44 App::MakeModelViewProjectionTransform() const
{
    const FMatrix44 Rotate = FMatrix44::RotateZ(Rotation);

    const FVector4 Eye(0, 0, 400, 1);
    const FVector4 Center(0, 0, 0, 1);
    const FVector4 Up(0, 1, 0, 0);
    const FMatrix44 View = FMatrix44::LookAt(Eye, Center, Up);

    const float FOV = DegreesToRadians(45.0f);
    const float Aspect = float(kRasterSize.x) / float(kRasterSize.y);
    const float Near = 0.1f;
    const float Far = 1000.0f;
    const FMatrix44 Projection = FMatrix44::Perspective(FOV, Aspect, Near, Far);

    const FMatrix44 Transform = Projection * View * Rotate;

    return Transform;
}

FVector4 App::ApplyTransform(const FMatrix44 &Transform, const FVector4 Vertex)
{
#if 0
    // C++ Vertex Transform
    // ------------------------

    // Transform by combined world+view+projection transform (-> Clip Space)
    const FVector4 VertexClipSpace = Transform * Vertex;

    // divide by W (-> NDC)
    const FVector4 VertexNDC = VertexClipSpace / VertexClipSpace.W;

    assert(VertexNDC.Z <= 1.0f);
    assert(VertexNDC.Z >= -1.0f);
    assert(VertexNDC.IsPoint());

    // Multiply by Screen Size + translate origin to center of screen
    const float kHalfRasterX = kRasterSize.x * 0.5f;
    const float kHalfRasterY = kRasterSize.y * 0.5f;
    
    const FVector4 VertexScreenSpace(
        (VertexNDC.X * kHalfRasterX) + kHalfRasterX,
        (VertexNDC.Y * kHalfRasterY) + kHalfRasterY,
        VertexNDC.Z,
        VertexNDC.W
    );
#else
    // Verilog Vertex Transform
    // ----------------------------

    HelperSetFixedPointMatrix(VertexTransform.i_matrix, Transform);
    HelperSetFixedPointVector(VertexTransform.i_vertex, Vertex);
    VertexTransform.i_screenWidth = ToFixedPoint(kRasterSize.x);
    VertexTransform.i_screenHeight = ToFixedPoint(kRasterSize.y);

    VertexTransform.eval();
    
    const FVector4 VertexScreenSpace = HelperGetFixedPointVector(VertexTransform.o_vertex);
#endif

    assert(VertexScreenSpace.Z <= 1.0f);
    assert(VertexScreenSpace.Z >= -1.0f);
    assert(VertexScreenSpace.IsPoint());

    return VertexScreenSpace;
}

void App::StartRenderingTriangle()
{    
    const FVector4 v[3] = {
        {-50, -60, 0, 1},
        {100, 100, 0, 1},
        {-100, 60, 0, 1}
    };

    const FMatrix44 Transform = MakeModelViewProjectionTransform();

    VlWide<4UL>* InputPorts[3] = {
        &Rasterizer.i_v1,
        &Rasterizer.i_v2,
        &Rasterizer.i_v3
    };

    for (int i=0; i<3; i++) 
    {
        const FVector4 VertexScreenSpace = ApplyTransform(Transform, v[i]);
        HelperSetFixedPointVector(*InputPorts[i], VertexScreenSpace);
    }

    Rasterizer.i_start = 1;
    StepRasterizer();
    Rasterizer.i_start = 0;
}

void App::ResetRasterizer()
{
    Rasterizer.i_reset_n = 0;
    Rasterizer.i_start = 0;
    Rasterizer.i_clk = 0;
    Rasterizer.eval();
    
    Rasterizer.i_reset_n = 1;
    Rasterizer.eval();
}

void App::StepRasterizer()
{
    Rasterizer.i_clk = 1;
    Rasterizer.eval();

    Rasterizer.i_clk = 0;
    Rasterizer.eval();
}

void App::Render()
{
    FillRect({ 0,0 }, { ScreenWidth(), ScreenHeight() }, olc::GREY);

    DrawRenderBuffer(kOriginBackBuffer, GetBackBuffer());
    DrawRenderBuffer(kOriginFrontBuffer, GetFrontBuffer());
}

void App::DrawRenderBuffer(const olc::vi2d& Origin, const std::vector<olc::Pixel>& RenderBuffer)
{
    for (int x=0; x<kRasterSize.x; x++)
    {
        for (int y=0; y<kRasterSize.y; y++)
        {
            const int Index = GetRenderBufferIndexForPixel(x, y);
            const olc::Pixel Pixel = RenderBuffer[Index];

            DrawRect(Origin + olc::vi2d{x,y}, {1,1}, Pixel);
        }
    }
}
