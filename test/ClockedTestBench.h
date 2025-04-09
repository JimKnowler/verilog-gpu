#pragma once

#include "TestBench.h"

template <class T>
class ClockedTestBench : public TestBench<T> {
public:
    ClockedTestBench()
    {
        Module.i_clock = 0;
        Module.i_reset_n = 1;
    }

    /**
     * @brief Tick the clock input on the module
     */
    void Tick() {
        Module.i_clock = 1;
        Eval();

        Module.i_clock = 0;
        Eval();
    }

    /**
     * @brief Reset the module
     */
    void Reset() {
        Module.i_reset_n = 0;

        Tick();

        Module.i_reset_n = 1;
    }
};
