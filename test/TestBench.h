#pragma once

template <class T>
class TestBench {
public:
    void Eval() {
        Module.eval();
    }

    // public access to Module, for setting inputs and getting outputs
    T Module;
};


