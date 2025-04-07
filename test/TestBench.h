#pragma once

template <class T>
class TestBench {
public:
    TestBench()
    {
        // ONLY REQUIRED IN MODULES THAT HAVE A CLOCK
        /*
        Module.i_clock = 0;
        */
    }

    ~TestBench()
    {

    }

    void Eval() {
        Module.eval();
    }

    // ONLY REQUIRED IN MODULES THAT HAVE A CLOCK
    /*
    void Tick() {
        Module.i_clock = 1;
        Eval();

        Module.i_clock = 0;
        Eval();
    }
    */

    // public access to Module, for setting inputs and getting outputs
    T Module;
};
