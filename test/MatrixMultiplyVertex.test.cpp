// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VMatrixMultiplyVertex.h"

#include "TestBench.h"
#include "gpu/FixedPoint.h"

class MatrixMultiplyVertex : public ::testing::Test {
public:
    TestBench<VMatrixMultiplyVertex> TestBench;

};

TEST_F(MatrixMultiplyVertex, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
    EXPECT_EQ(Module.i_matrix.size(), 4 * 4);
    EXPECT_EQ(Module.i_vertex.size(), 4);
    EXPECT_EQ(Module.o_vertex.size(), 4);
}

TEST_F(MatrixMultiplyVertex, ShouldMultiplyByZero)
{
    auto& Module = TestBench.Module;

    // create C++ zero Matrix
    // assign c++ matrix to i_matrix

    // create c++ vertex
    // assign c++ vetex to i_vertex

    TestBench.Eval();

    // test that output vertex is full of zeros
}


TEST_F(MatrixMultiplyVertex, ShouldMultiplyByIdentity)
{
    auto& Module = TestBench.Module;

    // create C++ identity Matrix
    // assign c++ matrix to i_matrix

    // create c++ vertex
    // assign c++ vetex to i_vertex

    TestBench.Eval();

    // test that output vertex matches the input
}
