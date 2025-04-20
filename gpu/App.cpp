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
            
    InitModel();
    InitAnimation();
    InitRasterizer();

    StartRenderingModel();

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
            if (!StartRenderingNextTriangle())
            {
                // frame complete
                SwapRenderBuffers();
                ClearBackBuffer();
                TickAnimation();
                StartRenderingModel();
            }

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
    const float RotationSpeed = M_PI / 2.0f;

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
    // orient the model in 3D space
    const FMatrix44 LocalTransform = FMatrix44::RotateX(DegreesToRadians(45.0f));
    
    // rotate the model around the Y axis
    const FMatrix44 WorldTransform = FMatrix44::RotateY(Rotation);

    const FVector4 Eye(0, 0, 400, 1);
    const FVector4 Center(0, 0, 0, 1);
    const FVector4 Up(0, 1, 0, 0);
    const FMatrix44 View = FMatrix44::LookAt(Eye, Center, Up);

    const float FOV = DegreesToRadians(45.0f);
    const float Aspect = float(kRasterSize.x) / float(kRasterSize.y);
    const float Near = 0.1f;
    const float Far = 1000.0f;
    const FMatrix44 Projection = FMatrix44::Perspective(FOV, Aspect, Near, Far);

    const FMatrix44 Transform = Projection * View * WorldTransform * LocalTransform;

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

void App::InitModel()
{
    InitModelCube();
}

void App::InitModelTriangle()
{
    const FVector4 Positions[3] = {
        {-50, -60, 0, 1},
        {100, 100, 0, 1},
        {-100, 60, 0, 1}
    };

    const FVector4 Colours[3] = {
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f}
    };

    for (int i=0; i<3; i++) 
    {
        IndexBuffer.push_back(i);
        VertexBuffer.push_back({
            .Position = Positions[i],
            .Colour = Colours[i]
        });
    }
}

namespace {
    std::vector<FVector4> kCubeVertices = {
        // Front face (+Z)
        {-1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},

        // Back face (-Z)
        { 1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f},

        // Left face (-X)
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f,  1.0f},
        {-1.0f,  1.0f,  1.0f},
        {-1.0f,  1.0f, -1.0f},

        // Right face (+X)
        { 1.0f, -1.0f,  1.0f},
        { 1.0f, -1.0f, -1.0f},
        { 1.0f,  1.0f, -1.0f},
        { 1.0f,  1.0f,  1.0f},

        // Top face (+Y)
        {-1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f,  1.0f},
        { 1.0f,  1.0f, -1.0f},
        {-1.0f,  1.0f, -1.0f},

        // Bottom face (-Y)
        {-1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f, -1.0f},
        { 1.0f, -1.0f,  1.0f},
        {-1.0f, -1.0f,  1.0f},
    };
}

void App::InitModelCube()
{
    const float Radius = 50.0f;

    const FVector4 Red(1.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 Green(0.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 Blue(0.0f, 0.0f, 1.0f, 1.0f);
    const FVector4 Yellow(1.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 Purple(1.0f, 0.0f, 1.0f, 1.0f);
    const FVector4 Gray(0.4f, 0.4f, 0.4f, 1.0f);

    const FVector4 kCubeFaceColours[6] = {
        Red,
        Green,
        Blue,
        Yellow,
        Purple,
        Gray
    };

    const int NumVertices = kCubeVertices.size();
    for (int i=0; i<NumVertices; i++)
    {
        const int Face = i / 4;

        const FVector4 Colour = kCubeFaceColours[Face];
        FVector4 Position = kCubeVertices[i] * Radius;
        Position.W = 1.0f;

        assert(Position.IsPoint());
        
        VertexBuffer.push_back({
            .Position = Position,
            .Colour = Colour
        });
    }

    NumTriangles = 12;

    IndexBuffer = {
        // Front face
        0,  1,  2,
        0,  2,  3,

        // Back face
        4,  5,  6,
        4,  6,  7,

        // Left face
        8,  9, 10,
        8, 10, 11,

        // Right face
        12, 13, 14,
        12, 14, 15,

        // Top face
        16, 17, 18,
        16, 18, 19,

        // Bottom face
        20, 21, 22,
        20, 22, 23,
    };
}

void App::StartRenderingModel()
{
    TriangleIndex = 0;

    RenderTriangle(TriangleIndex);
}

void App::RenderTriangle(int Index)
{
    const int ArrayIndex = Index * 3;

    const int Indexes[3] = {
        IndexBuffer[ArrayIndex],
        IndexBuffer[ArrayIndex + 1],
        IndexBuffer[ArrayIndex + 2]
    };

    const FVertex v[3] = {
        VertexBuffer[Indexes[0]],
        VertexBuffer[Indexes[1]],
        VertexBuffer[Indexes[2]],
    };

    const FMatrix44 Transform = MakeModelViewProjectionTransform();

    VlWide<4UL>* VertexInputPorts[3] = {
        &Rasterizer.i_v1,
        &Rasterizer.i_v2,
        &Rasterizer.i_v3
    };

    VlWide<4UL>* ColourInputPorts[3] = {
        &Rasterizer.i_c1,
        &Rasterizer.i_c2,
        &Rasterizer.i_c3
    };

    for (int i=0; i<3; i++) 
    {
        const FVector4 VertexScreenSpace = ApplyTransform(Transform, v[i].Position);
        HelperSetFixedPointVector(*VertexInputPorts[i], VertexScreenSpace);

        HelperSetFixedPointVector(*ColourInputPorts[i], v[i].Colour);
    }

    Rasterizer.i_start = 1;
    StepRasterizer();
    Rasterizer.i_start = 0;

    // verify that rasterizer is caching vertex locations + colours

    for (int i=0; i<3; i++) 
    {
        HelperSetFixedPointVector(*VertexInputPorts[i], FVector4::Zero());
        HelperSetFixedPointVector(*ColourInputPorts[i], FVector4::Zero());
    }
}

bool App::StartRenderingNextTriangle()
{
    TriangleIndex += 1;
    if (TriangleIndex >= NumTriangles)
    {
        return false;
    }

    RenderTriangle(TriangleIndex);
    
    return true;
}

void App::InitRasterizer()
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
