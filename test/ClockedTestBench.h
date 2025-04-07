#pragma once

#include "TestBench.h"

template <class T>
class ClockedTestBench : public TestBench<T> {
public:
    ClockedTestBench()
    {
        Module.i_clock = 0;

    }

    void Tick() {
        Module.i_clock = 1;
        Eval();

        Module.i_clock = 0;
        Eval();
    }
};
