// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VUnsignedDivide.h"

#include "ClockedTestBench.h"

class UnsignedDivide : public ::testing::Test {
public:
    ClockedTestBench<VUnsignedDivide> TestBench;

    void HelperTestDivide(const int Dividend, const int Divisor) 
    {
        TestBench.Reset();
        auto& Module = TestBench.Module;

        Module.i_dividend = Dividend;
        Module.i_divisor = Divisor;
        Module.i_start = 1;
        TestBench.Tick();

        Module.i_dividend = 0;
        Module.i_divisor = 0;
        Module.i_start = 0;

        int TickCount = 0;
        while (Module.o_valid == 0)
        {
            ASSERT_EQ(Module.o_ready, 0);
            TestBench.Tick();           

            // prevent infinite loop
            TickCount += 1;
            ASSERT_LT(TickCount, 100);
        }
        
        EXPECT_EQ(Module.o_ready, 1);
        
        TestBench.Tick();
        EXPECT_EQ(Module.o_ready, 1);
        EXPECT_EQ(Module.o_valid, 0);

        const int ExpectedQuotient = Dividend / Divisor;
        const int ActualQuotient = Module.o_quotient;
        EXPECT_EQ(ExpectedQuotient, ActualQuotient);
        
        const int ExpectedRemainder = Dividend % Divisor;
        const int ActualRemainder = Module.o_remainder;
        EXPECT_EQ(ExpectedRemainder, ActualRemainder);
    }
};

TEST_F(UnsignedDivide, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
}

TEST_F(UnsignedDivide, ShouldReset)
{
    TestBench.Reset();

    auto& Module = TestBench.Module;
    
    EXPECT_EQ(Module.o_ready, 1);
    EXPECT_EQ(Module.o_valid, 0);
}

TEST_F(UnsignedDivide, ShouldDivideTwelveBySeven)
{
    HelperTestDivide(12, 7);
}

TEST_F(UnsignedDivide, ShouldDivideTwelveBySix)
{
    HelperTestDivide(12, 6);
}

TEST_F(UnsignedDivide, ShouldDivideTwelveByFive)
{
    HelperTestDivide(12, 5);
}
