#include "Rasterizers.h"

#include <assert.h>

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

    const olc::vi2d kTextOffset(0, 10);

    std::vector<olc::Pixel> RenderBufferBoundingBox(kRasterBufferSize, olc::BLACK);
    RasterizeBoundingBox(Triangle, RenderBufferBoundingBox);
    DrawRenderBuffer(kOriginBoundingBox + kTextOffset, RenderBufferBoundingBox);
    DrawString(kOriginBoundingBox, "Bounding Box", olc::BLACK);

    std::vector<olc::Pixel> RenderBufferZigZag(kRasterBufferSize, olc::BLACK);
    RasterizeZigZag(Triangle, RenderBufferZigZag);
    DrawRenderBuffer(kOriginZigZag + kTextOffset, RenderBufferZigZag);
    DrawString(kOriginZigZag, "Zig Zag", olc::BLACK);
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

    // find starting position
    olc::vi2d start;
    GetStartingVertex({Triangle.v1, Triangle.v2, Triangle.v3}, start);
    int x = start.x;
    int y = start.y;

    // initialise edge functions based on starting x,y
    int e1 = (a1 * x) + (b1 * y) + c1;
    int e2 = (a2 * x) + (b2 * y) + c2;
    int e3 = (a3 * x) + (b3 * y) + c3;

    // find finishing row
    const int MaxY = std::max(Triangle.v1.y, std::max(Triangle.v2.y, Triangle.v3.y));

    bool bIsMovingRight = true;
    int dx = 1;

    while (true)
    {
        // determine if pixel is inside the triangle
        // TODO: implement logic for handling edge function == 0, at edge of adjoining triangles
        const bool bIsInside = (e1 >=0) && (e2 >= 0) && (e3 >= 0);

        if (bIsInside)
        {
            // rasterize pixel
            const int PixelIndex = GetRenderBufferPixelIndex(x, y);
            RenderBuffer[PixelIndex] = olc::GREEN;
        }

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
            if (y > MaxY)
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
    auto EdgeFunction = [](const olc::vi2d& v1, const olc::vi2d& v2, int& a, int& b, int& c) -> void {
        a = v1.y - v2.y;
        b = v2.x - v1.x;
        c = (v1.x * v2.y) - (v2.x * v1.y);
    };

    int a1, b1, c1, a2, b2, c2, a3, b3, c3;
    EdgeFunction(Triangle.v2, Triangle.v3, a1, b1, c1);
    EdgeFunction(Triangle.v3, Triangle.v1, a2, b2, c2);
    EdgeFunction(Triangle.v1, Triangle.v2, a3, b3, c3);

    // find starting position
    olc::vi2d start;
    GetStartingVertex({Triangle.v1, Triangle.v2, Triangle.v3}, start);
    int x = start.x;
    int y = start.y;

    // find finishing row
    const int MaxY = std::max(Triangle.v1.y, std::max(Triangle.v2.y, Triangle.v3.y));

    // initialise edge functions based on starting x,y
    int e1 = (a1 * x) + (b1 * y) + c1;
    int e2 = (a2 * x) + (b2 * y) + c2;
    int e3 = (a3 * x) + (b3 * y) + c3;

    bool bIsMovingRight = true;
    int dx = 1;

    enum class EState {
        FindExternalPixel,
        Rasterizing
    };

    EState State = EState::Rasterizing;

    constexpr size_t s_fully_inside = 1 | (1 << 1) | (1 << 2);
    size_t s_stored = s_fully_inside;

    while (true)
    {
        assert(x >= 0);
        assert(x < kRasterSize.x);
        assert(y >= 0);
        assert(y < kRasterSize.y);

        const size_t s_current = (((e1 >= 0) ? 1 : 0) << 0)
                               | (((e2 >= 0) ? 1 : 0) << 1)
                               | (((e3 >= 0) ? 1 : 0) << 2);

        if (s_current == s_fully_inside)
        {
            const int PixelIndex = GetRenderBufferPixelIndex(x, y);
            RenderBuffer[PixelIndex] = olc::YELLOW;
        }

        switch (State)
        {
            case EState::FindExternalPixel:
                {
                    // moving horizontally, while inside the triangle, until outside the triangle
                    const size_t inv_s_current = (~s_current) & s_fully_inside;
                    const size_t s_combined = (inv_s_current | s_stored);

                    if (s_combined == s_fully_inside)
                    {
                        // this pixel has s_current with zeros in at least the same places as the s_stored value
                        
                        // we have searched past the end of the 'inside' pixels

                        // switch horizontal direction
                        dx = -dx;
                        a1 = -a1;
                        a2 = -a2;
                        a3 = -a3;

                        State = EState::Rasterizing;
                        
                    }
                    else
                    {
                        // keep searching in the current direction for the first pixel 
                        // that is 'outside'
                        x += dx;

                        e1 += a1;
                        e2 += a2;
                        e3 += a3;    
                    }                    
                }
                break;
            case EState::Rasterizing:
                {
                    // moving horizontally inside the triangle
                    if ((s_current != s_fully_inside) && ((s_current | s_stored) == s_fully_inside))
                    {
                        // we have finished rasterizing this row
                        // s_current has ones at least in the same places where s_stored had zeros, and is not fully inside
                    
                        // start the next row
                        y += 1;
                        if (y > MaxY)
                        {
                            return;
                        }

                        // continue in the same horizontal direction, while we search for an external pixel
                        State = EState::FindExternalPixel;
                        
                        e1 += b1;
                        e2 += b2;
                        e3 += b3;

                        // store s for the external pixel on this row
                        s_stored = s_current;
                    } else {
                        x += dx;

                        e1 += a1;
                        e2 += a2;
                        e3 += a3;
                    }
                }
        }
    }
}

void Rasterizers::GetStartingVertex(const std::vector<olc::vi2d>& vertices, olc::vi2d& v) {
    int x = INT_MAX;
    int y = INT_MAX;

    for (const olc::vi2d& test: vertices)
    {
        if ((test.y < y) || ((test.y == y) && (test.x < x)))
        {
            x = test.x;
            y = test.y;
        }
    }

    v.x = x;
    v.y = y;
};
