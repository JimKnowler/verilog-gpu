#include "App.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

namespace {
    const uint32_t kScreenWidth = 1300;
    const uint32_t kScreenHeight = 700;
}

class App : public olc::PixelGameEngine
{
public:
    App() {
        sAppName = "verilog-gpu";
    }

    /// @brief called once at start
    /// @return true, if successful
    bool OnUserCreate() override;

    // @brief called every frame
    /// @return true, if successful
    bool OnUserUpdate(float fElapsedTime) override;

private:
    void Update(float DeltaTime);
    void Render();
};

bool App::OnUserCreate() {
    printf("OnUserCreate\n");

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
}

void App::Render()
{
    FillRect({ 0,0 }, { ScreenWidth(), ScreenHeight() }, olc::GREY);
    DrawRect({100,100}, {200,100}, olc::RED);
}

void RunApp()
{
    App app;

    if (app.Construct(kScreenWidth, kScreenHeight, 1, 1))
        app.Start();
}