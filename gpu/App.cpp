#include "App.h"
#include "FixedPoint.h"
#include "VerilatorHelpers.h"

// whether to use the landscape model instead of the cube
#define ENABLE_MODEL_LANDSCAPE 1

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

    // View Transform
    const FVector4 Eye(0, 0, 400, 1);               // location of camera
    const FVector4 Center(0, 0, 0, 1);              // location that camera is pointing towards
    const FVector4 Up(0, 1, 0, 0);                  // camera's up vector

    // Directional light
    const FVector4 kLightDirection = FVector4(1.0f, 0.5f, -1.0f).Normalise();

    // Cube model size
    const float kCubeRadius = 50.0f;
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
    InitTriangleAssembly();
    InitMemory();

    StartTriangleAssembly();

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
        if (Rasterizer.o_ready && TriangleAssembly.o_ready)
        {
            // frame complete
            
            SwapRenderBuffers();
            ClearBackBuffer();
            TickAnimation();
            StartTriangleAssembly();
        
            continue;
        }        

        if (Rasterizer.o_write)
        {
            RenderPixelFromRasterizerToBackBuffer();
        } 

        StepRasterizer();
        StepTriangleAssembly();
    }
}

void App::RenderPixelFromRasterizerToBackBuffer()
{
    const int x = Rasterizer.o_x;
    const int y = Rasterizer.o_y;

#if 0
    printf("Render Pixel at [%d] , [%d]\n", x, y);
#endif

    const int Index = GetRenderBufferPixelIndex(x, y);
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

int App::GetRenderBufferPixelIndex(int x, int y) const
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
#if ENABLE_MODEL_LANDSCAPE
    const float RotationSpeed = M_PI / 10.0f;
#else
    const float RotationSpeed = M_PI / 2.0f;
#endif

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

FMatrix44 App::MakeWorldTransform() const
{
#if ENABLE_MODEL_LANDSCAPE
    const FMatrix44 Transform = FMatrix44::RotateX(DegreesToRadians(-45.0f)) * FMatrix44::RotateY(Rotation) * FMatrix44::RotateX(DegreesToRadians(90.0f));
#else
    // orient the model in 3D space
    const FMatrix44 LocalTransform = FMatrix44::RotateX(DegreesToRadians(45.0f));
    
    // rotate the model
    const FMatrix44 WorldTransform = FMatrix44::RotateY(Rotation) * FMatrix44::RotateZ(Rotation);
    
    const FMatrix44 Transform = WorldTransform * LocalTransform;
#endif

    return Transform;
}

FMatrix44 App::MakeViewProjectionTransform() const
{
    const FMatrix44 View = FMatrix44::LookAt(Eye, Center, Up);

    const float FOVDegrees = 45.0f;
    const float FOV = DegreesToRadians(FOVDegrees);
    const float Aspect = float(kRasterSize.x) / float(kRasterSize.y);
    const float Near = 0.1f;
    const float Far = 1000.0f;
    const FMatrix44 Projection = FMatrix44::Perspective(FOV, Aspect, Near, Far);

    const FMatrix44 Transform = Projection * View;

    return Transform;
}

void App::InitModel()
{
#if ENABLE_MODEL_LANDSCAPE
    Model.InitLandscape();
#else
    Model.InitCube(kCubeRadius);
#endif
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
    Rasterizer.i_start = TriangleAssembly.o_valid;
    Rasterizer.i_v1 = TriangleAssembly.o_v1;
    Rasterizer.i_v2 = TriangleAssembly.o_v2;
    Rasterizer.i_v3 = TriangleAssembly.o_v3;
    Rasterizer.i_c1 = TriangleAssembly.o_c1;
    Rasterizer.i_c2 = TriangleAssembly.o_c2;
    Rasterizer.i_c3 = TriangleAssembly.o_c3;

    if (Rasterizer.i_start)
    {
        assert(Rasterizer.o_ready);
    }
    
    Rasterizer.i_clk = 1;
    Rasterizer.eval();

    Rasterizer.i_clk = 0;
    Rasterizer.eval();

    if (Rasterizer.i_start)
    {
        assert(!Rasterizer.o_ready);
    }
}

void App::InitTriangleAssembly()
{
    // Reset module

    TriangleAssembly.i_reset_n = 0;
    TriangleAssembly.i_start = 0;
    TriangleAssembly.i_clk = 0;
    TriangleAssembly.eval();
    
    TriangleAssembly.i_reset_n = 1;
    TriangleAssembly.eval();
}

void App::InitMemory()
{
    // Prepare memory for Triangle Assembly

    const std::vector<uint32_t>& IndexBuffer = Model.IndexBuffer;
    const std::vector<FFixedPointVertex>& VertexBuffer = Model.VertexBuffer;

    const size_t kIndexBufferSize = IndexBuffer.size() * sizeof(uint32_t);
    const size_t kVertexBufferSize = VertexBuffer.size() * sizeof(FFixedPointVertex);

    const size_t kMemorySize = (kIndexBufferSize + kVertexBufferSize) / sizeof(uint32_t);
    Memory = std::vector<uint32_t>(kMemorySize, 0);

    memcpy(&Memory.front(), &IndexBuffer.front(), kIndexBufferSize);
    memcpy(reinterpret_cast<uint8_t*>(&Memory.front()) + kIndexBufferSize, &VertexBuffer.front(), kVertexBufferSize);
}

void App::StartTriangleAssembly()
{
    // start rendering the model stored in the index + vertex buffers
    
    const std::vector<uint32_t>& IndexBuffer = Model.IndexBuffer;
    const int NumTriangles = Model.NumTriangles;    

    const int AddressIndexBuffer = 0;
    const int AddressVertexBuffer = IndexBuffer.size() * sizeof(uint32_t);

    TriangleAssembly.i_address_index_buffer = AddressIndexBuffer;
    TriangleAssembly.i_address_vertex_buffer = AddressVertexBuffer;    
    TriangleAssembly.i_num_triangles = NumTriangles;
    
    HelperSetFixedPointMatrix(TriangleAssembly.i_world, MakeWorldTransform());
    HelperSetFixedPointMatrix(TriangleAssembly.i_view_projection, MakeViewProjectionTransform());
    
    TriangleAssembly.i_screenWidth = ToFixedPoint(kRasterSize.x);
    TriangleAssembly.i_screenHeight = ToFixedPoint(kRasterSize.y);
    HelperSetFixedPointVector(TriangleAssembly.i_light_direction, kLightDirection);
    TriangleAssembly.i_start = 1;
    TriangleAssembly.i_memory_data = 0;

    StepTriangleAssembly();

    // Reset inputs that only need to be valid while 'i_start' is raised
    TriangleAssembly.i_address_vertex_buffer = 0;
    TriangleAssembly.i_address_index_buffer = 0;
    TriangleAssembly.i_num_triangles = 0;
    HelperSetFixedPointMatrix(TriangleAssembly.i_world, FMatrix44::Zero());
    HelperSetFixedPointMatrix(TriangleAssembly.i_view_projection, FMatrix44::Zero());
    TriangleAssembly.i_screenWidth = 0;
    TriangleAssembly.i_screenHeight = 0;
    HelperSetFixedPointVector(TriangleAssembly.i_light_direction, FVector4());
    TriangleAssembly.i_start = 0;

    StepTriangleAssembly();

    assert(TriangleAssembly.o_ready == 0);
    assert(TriangleAssembly.o_debug_triangle_index == 0);
    assert(TriangleAssembly.o_debug_num_triangles == NumTriangles);
}

void App::StepTriangleAssembly()
{
    TriangleAssembly.i_rasterizer_ready = Rasterizer.o_ready;

    if (TriangleAssembly.o_memory_read)
    {
        const size_t MemoryAddress = TriangleAssembly.o_memory_address;
        TriangleAssembly.i_memory_data = Memory[MemoryAddress / 4];        
    }
    else
    {
        TriangleAssembly.i_memory_data = 0;
    }

    TriangleAssembly.i_clk = 1;
    TriangleAssembly.eval();

    TriangleAssembly.i_clk = 0;
    TriangleAssembly.eval();
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
            const int Index = GetRenderBufferPixelIndex(x, y);
            const olc::Pixel Pixel = RenderBuffer[Index];

            DrawRect(Origin + olc::vi2d{x,y}, {1,1}, Pixel);
        }
    }
}
