// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VMatrixMultiplyVector.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"

class MatrixMultiplyVector : public ::testing::Test {
public:
    TestBench<VMatrixMultiplyVector> TestBench;

};

TEST_F(MatrixMultiplyVector, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_matrix.size(), 4 * 4);
    EXPECT_EQ(Module.i_vector.size(), 4);
    EXPECT_EQ(Module.o_vector.size(), 4);
}

TEST_F(MatrixMultiplyVector, ShouldMultiplyByZero)
{
    auto& Module = TestBench.Module;

    // create C++ zero Matrix
    // assign c++ matrix to i_matrix

    // create c++ vector
    // assign c++ vetex to i_vector

    TestBench.Eval();

    // test that output vector is full of zeros
}


TEST_F(MatrixMultiplyVector, ShouldMultiplyByIdentity)
{
    auto& Module = TestBench.Module;

    // create C++ identity Matrix
    // assign c++ matrix to i_matrix

    // create c++ vector
    // assign c++ vetex to i_vector

    TestBench.Eval();

    // test that output vector matches the input
}
