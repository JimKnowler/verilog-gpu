#pragma once

#include "TestBench.h"

template <class T>
class ClockedTestBench : public TestBench<T> {
public:
    ClockedTestBench()
    {
        TestBench<T>::Module.i_clk = 0;
        TestBench<T>::Module.i_reset_n = 1;
    }

    /**
     * @brief Tick the clock input on the module
     */
    void Tick() {
        TestBench<T>::Module.i_clk = 1;
        TestBench<T>::Eval();

        TestBench<T>::Module.i_clk = 0;
        TestBench<T>::Eval();
    }

    /**
     * @brief Reset the module
     */
    void Reset() {
        TestBench<T>::Module.i_reset_n = 0;
        
        Tick();
        
        TestBench<T>::Module.i_reset_n = 1;
    }
};
