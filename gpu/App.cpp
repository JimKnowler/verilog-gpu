#include "App.h"
#include "FixedPoint.h"
#include "VerilatorHelpers.h"

namespace {
    const uint32_t kScreenWidth = 1280;
    const uint32_t kScreenHeight = 720;

    const olc::vi2d kRasterOrigin = {400, 150};
    const olc::vi2d kRasterSize = {400, 400};

    const int kRasterizerBatchSize = 10000;

    // vertex colours
    olc::Pixel c1{255, 0, 0};
    olc::Pixel c2{0, 255, 0};
    olc::Pixel c3{0, 0, 255};

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
    Rasterizer.i_v1r = ToFixedPoint(c1.r / 255.0f);
    Rasterizer.i_v1g = ToFixedPoint(c1.g / 255.0f);
    Rasterizer.i_v1b = ToFixedPoint(c1.b / 255.0f);
    
    Rasterizer.i_v2r = ToFixedPoint(c2.r / 255.0f);
    Rasterizer.i_v2g = ToFixedPoint(c2.g / 255.0f);
    Rasterizer.i_v2b = ToFixedPoint(c3.b / 255.0f);

    Rasterizer.i_v3r = ToFixedPoint(c3.r / 255.0f);
    Rasterizer.i_v3g = ToFixedPoint(c3.g / 255.0f);
    Rasterizer.i_v3b = ToFixedPoint(c3.b / 255.0f);

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

    const int r = Rasterizer.o_r;
    const int g = Rasterizer.o_g;
    const int b = Rasterizer.o_b;

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

    const FVector4 v[3] = {
        {-50, -60, 0, 1},
        {100, 100, 0, 1},
        {-100, 60, 0, 1}
    };

    const FMatrix44 Transform = MakeModelViewProjectionTransform();

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

        switch (i)
        {
            case 0:
                Rasterizer.i_v1x = ToFixedPoint(Vert.X);
                Rasterizer.i_v1y = ToFixedPoint(Vert.Y);
                break;
    
            case 1:
                Rasterizer.i_v2x = ToFixedPoint(Vert.X);
                Rasterizer.i_v2y = ToFixedPoint(Vert.Y);
                break;
    
            case 2:
                Rasterizer.i_v3x = ToFixedPoint(Vert.X);
                Rasterizer.i_v3y = ToFixedPoint(Vert.Y); 
                break;
        };
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
