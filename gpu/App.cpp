#include "App.h"
#include "FixedPoint.h"
#include "VerilatorHelpers.h"

namespace {
    const uint32_t kScreenWidth = 1280;
    const uint32_t kScreenHeight = 720;

    const olc::vi2d kRasterOrigin = {400, 150};
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

    // Vertex colours
    HelperSetFixedPointVector(Rasterizer.i_c1, c1);
    HelperSetFixedPointVector(Rasterizer.i_c2, c2);
    HelperSetFixedPointVector(Rasterizer.i_c3, c3);
        
    InitRotateTriangle();

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
    Rasterizer.i_x = x;
    Rasterizer.i_y = y;

    Rasterizer.eval();

    const int Index = GetRenderBufferIndex(x, y);
    olc::Pixel& Pixel = RenderBuffer[Index];

    if (Rasterizer.o_write == 1)
    {
        const FVector4 Colour = HelperGetFixedPointVector(Rasterizer.o_colour);

        const std::function FloatToUint8 = [](float Value) -> uint8_t {
            return static_cast<uint8_t>(255.0f * std::clamp(Value, 0.0f, 1.0f));
        };

        const uint8_t r = FloatToUint8(Colour.X);
        const uint8_t g = FloatToUint8(Colour.Y);
        const uint8_t b = FloatToUint8(Colour.Z);

        Pixel = olc::Pixel(r, g, b);
    } else {
        Pixel = olc::BLACK;
    }
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
        FVector4 Vert = v[i];

#if 0
        // C++ Vertex Transform
        // ------------------------

        // Transform by combined world+view+projection transform (-> Clip Space)
        Vert = Transform * Vert;

        // divide by W (-> NDC)
        Vert = Vert / Vert.W;

        // Multiply by Screen Size + translate origin to center of screen
        const float kHalfRasterX = kRasterSize.x * 0.5f;
        const float kHalfRasterY = kRasterSize.y * 0.5f;
        Vert.X = (Vert.X * kHalfRasterX) + kHalfRasterX;
        Vert.Y = (Vert.Y * kHalfRasterY) + kHalfRasterY;

        assert(Vert.Z < 1.0f);
        assert(Vert.Z > -1.0f);
#else
        // Verilog Vertex Transform
        // ----------------------------

        HelperSetFixedPointMatrix(VertexTransform.i_matrix, Transform);
        HelperSetFixedPointVector(VertexTransform.i_vertex, Vert);
        VertexTransform.i_screenWidth = ToFixedPoint(kRasterSize.x);
        VertexTransform.i_screenHeight = ToFixedPoint(kRasterSize.y);

        VertexTransform.eval();
        
        Vert = HelperGetFixedPointVector(VertexTransform.o_vertex);
#endif

        HelperSetFixedPointVector(*InputPorts[i], Vert);
    }

    
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

void App::Render()
{
    FillRect({ 0,0 }, { ScreenWidth(), ScreenHeight() }, olc::GREY);

    for (int x=0; x<kRasterSize.x; x++)
    {
        for (int y=0; y<kRasterSize.y; y++)
        {
            const int Index = GetRenderBufferIndex(x, y);
            const olc::Pixel Pixel = RenderBuffer[Index];

            DrawRect(kRasterOrigin + olc::vi2d{x,y}, {1,1}, Pixel);
        }
    }
}
