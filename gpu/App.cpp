#include "App.h"

namespace {
    const uint32_t kScreenWidth = 1300;
    const uint32_t kScreenHeight = 700;
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

App::App() {
    sAppName = "verilog-gpu";
}

bool App::OnUserCreate() {
    printf("OnUserCreate\n");

    // >>
    // Construct a VerilatedContext to hold simulation time, etc.
    VerilatedContext* const contextp = new VerilatedContext;

    // Pass arguments so Verilated code can see them, e.g. $value$plusargs
    // This needs to be called before you create any model
    // contextp->commandArgs(argc, argv);

    // enable internal verilator state for waveform tracing
    // - note : do this before creating module, so that we have the option of 
    //          starting a trace at another time.  
    //          See VerilatedVcdC, top->trace(), trace->open(), trace->dump(), and trace->close()
    // contextp->traceEverOn(true);

    // set debug level
    // contextp->debug(1);

    // Construct the Verilated model, from Vtop.h generated from Verilating "top.v"
    VTop* const top = new VTop{contextp};

    top->a = 3;
    top->b = 5;
    top->eval();

    printf("model result is [%d]\n", top->sum);

    // Final model cleanup
    top->final();

    // Destroy model
    delete top;
    delete contextp;
    // <<

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
