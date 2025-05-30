// GoogleTest
#include <gtest/gtest.h>

// Verilator
#include <verilated.h>

// Verilated Module
#include "VTriangleAssembly.h"

#include "ClockedTestBench.h"
#include "gpu/FixedPoint.h"
#include "MathTestHelpers.h"
#include "gpu/VerilatorHelpers.h"
#include "gpu/FixedPointVertex.h"

// flag to enable logging of the TestBench module's state
#define ENABLE_LOGGING 0

class TriangleAssembly : public ::testing::Test {
public:
    ClockedTestBench<VTriangleAssembly> TestBench;
    std::vector<uint32_t> Memory;

    void HelperResetTestBench()
    {
        TestBench.Reset();

        auto& Module = TestBench.Module;

#if ENABLE_LOGGING
        printf("Tick [Reset] State [%d] StateCounter [%d] TriangleIndex [%d]\n", 
            Module.o_debug_state, Module.o_debug_state_counter, 
            Module.o_debug_triangle_index);
#endif
    }

    void HelperPrepareMemory(const std::vector<uint32_t>& IndexBuffer, const std::vector<FFixedPointVertex>& VertexBuffer)
    {
        const size_t kIndexBufferSize = IndexBuffer.size() * sizeof(uint32_t);
        const size_t kVertexBufferSize = VertexBuffer.size() * sizeof(FFixedPointVertex);

        const size_t kMemorySize = (kIndexBufferSize + kVertexBufferSize) / sizeof(uint32_t);
        Memory = std::vector<uint32_t>(kMemorySize, 0);
        ASSERT_EQ(kMemorySize, Memory.size());

        memcpy(&Memory.front(), &IndexBuffer.front(), kIndexBufferSize);
        memcpy(reinterpret_cast<uint8_t*>(&Memory.front()) + kIndexBufferSize, &VertexBuffer.front(), kVertexBufferSize);
    }

    void HelperStartModule(int AddressIndexBuffer, int AddressVertexBuffer, int NumTriangles)
    {
        auto& Module = TestBench.Module;

        Module.i_address_index_buffer = AddressIndexBuffer;
        Module.i_address_vertex_buffer = AddressVertexBuffer;    
        Module.i_num_triangles = NumTriangles;
        
        HelperSetFixedPointMatrix(Module.i_world, FMatrix44::Identity());
        
        const FVector4 Eye{0.0f, 0.0f, 100.0f, 1.0f};
        const FVector4 Center{0.0f, 0.0f, 0.0f, 1.0f};
        const FVector4 Up{0.0f, 1.0f, 0.0f, 0.0f};
        const FMatrix44 kViewProjection = FMatrix44::Perspective(DegreesToRadians(45), 1.0f, 0.1f, 1000.0f)
                                        * FMatrix44::LookAt(Eye, Center, Up);
        HelperSetFixedPointMatrix(Module.i_view_projection, kViewProjection);
        
        Module.i_screenWidth = 600;
        Module.i_screenHeight = 400;
        Module.i_start = 1;

        Module.i_rasterizer_ready = 1;
        Module.i_memory_data = 0;

        TestBench.Tick();

        // Reset inputs that only need to be valid while 'i_start' is raised
        Module.i_address_vertex_buffer = 0;
        Module.i_address_index_buffer = 0;
        Module.i_num_triangles = 0;
        HelperSetFixedPointMatrix(Module.i_world, FMatrix44::Zero());
        HelperSetFixedPointMatrix(Module.i_view_projection, FMatrix44::Zero());
        Module.i_screenWidth = 0;
        Module.i_screenHeight = 0;
        Module.i_start = 0;
    }

    bool HelperTickModuleUntilValidOrReady(int MaxNumTicks)
    {
        auto& Module = TestBench.Module;

        for (int i=0; i<MaxNumTicks; i++)
        {
#if ENABLE_LOGGING
            printf("Tick [%d] State [%d] StateCounter [%d] TriangleIndex [%d]\n", 
                i, 
                Module.o_debug_state, 
                Module.o_debug_state_counter, 
                Module.o_debug_triangle_index
            );
#endif

#if ENABLE_LOGGING
            printf("  - v1[%s] v2[%s] v3[%s]\n",
                HelperGetFixedPointVector(Module.o_v1).ToString().c_str(),
                HelperGetFixedPointVector(Module.o_v2).ToString().c_str(),
                HelperGetFixedPointVector(Module.o_v3).ToString().c_str()
            );
#endif
            
            if (Module.o_valid || Module.o_ready) 
            {
                return true;
            }
            
            if (Module.o_memory_read)
            {
                Module.i_memory_data = Memory[Module.o_memory_address / 4];
            }
            else
            {
                Module.i_memory_data = 12345678;
            }

#if ENABLE_LOGGING
            printf("  - o_memory_read[%u] o_memory_address[%u] i_memory_data[%u]\n",
                Module.o_memory_read,
                Module.o_memory_address,
                Module.i_memory_data
            );
#endif

            TestBench.Tick();
        }

        return false;
    }
};

TEST_F(TriangleAssembly, ShouldConstructTestBench)
{
    auto& Module = TestBench.Module;
}

TEST_F(TriangleAssembly, ShouldReset)
{
    HelperResetTestBench();

    auto& Module = TestBench.Module;

    EXPECT_EQ(1, Module.o_ready);
    EXPECT_EQ(0, Module.o_error);
    EXPECT_EQ(0, Module.o_valid);
    EXPECT_EQ(0, Module.o_memory_read);
}

TEST_F(TriangleAssembly, ShouldRenderSingleTriangle)
{
    HelperResetTestBench();

    auto& Module = TestBench.Module;

    // prepare a triangle for render
    std::vector<uint32_t> IndexBuffer = {0, 1, 2};
    std::vector<FFixedPointVertex> VertexBuffer = {
        {
            .Position = FFixedPointVector4(10.0f, 0.0f, 0.0f, 1.0f),
            .Normal = FFixedPointVector4(0.0f, 0.0f, 1.0f, 0.0f),
            .Colour = FFixedPointVector4(1.0f, 0.0f, 0.0f, 1.0f)
        },
        {
            .Position = FFixedPointVector4{100.0f, 0.0f, 0.0f, 1.0f},
            .Normal = FFixedPointVector4{0.0f, 0.0f, 1.0f, 0.0f},
            .Colour = FFixedPointVector4{0.0f, 1.0f, 0.0f, 1.0f}
        },
        {
            .Position = FFixedPointVector4{50.0f, 50.0f, 0.0f, 1.0f},
            .Normal = FFixedPointVector4{0.0f, 0.0f, 1.0f, 0.0f},
            .Colour = FFixedPointVector4{0.0f, 0.0f, 1.0f, 1.0f}
        }
    };

    HelperPrepareMemory(IndexBuffer, VertexBuffer);

    const uint32_t kIndexBufferSize = IndexBuffer.size() * sizeof(uint32_t);
    HelperStartModule(0, kIndexBufferSize, 1);
    
    const int kMaxNumTicks = 100;
    EXPECT_TRUE(HelperTickModuleUntilValidOrReady(kMaxNumTicks));

    // state = START_RASTERIZER
    ASSERT_EQ(1, Module.o_valid);
    ASSERT_EQ(0, Module.o_ready);

    TestBench.Tick();

    // state = TRIANGLE_COMPLETE
    ASSERT_EQ(0, Module.o_valid);
    ASSERT_EQ(0, Module.o_ready);

    TestBench.Tick();

    // state = READY
    ASSERT_EQ(0, Module.o_valid);
    ASSERT_EQ(1, Module.o_ready);
    ASSERT_EQ(1, Module.o_debug_triangle_index);
}

TEST_F(TriangleAssembly, ShouldNotRasterizeSingleTriangleFacingBackwards)
{
    HelperResetTestBench();

    auto& Module = TestBench.Module;

    // prepare a triangle for render
    std::vector<uint32_t> IndexBuffer = {0, 2, 1};          // NOTE: change winding order,
                                                            //   to force back face culling
    std::vector<FFixedPointVertex> VertexBuffer = {
        {
            .Position = FFixedPointVector4(10.0f, 0.0f, 0.0f, 1.0f),
            .Normal = FFixedPointVector4(0.0f, 0.0f, 1.0f, 0.0f),
            .Colour = FFixedPointVector4(1.0f, 0.0f, 0.0f, 1.0f)
        },
        {
            .Position = FFixedPointVector4{100.0f, 0.0f, 0.0f, 1.0f},
            .Normal = FFixedPointVector4{0.0f, 0.0f, 1.0f, 0.0f},
            .Colour = FFixedPointVector4{0.0f, 1.0f, 0.0f, 1.0f}
        },
        {
            .Position = FFixedPointVector4{50.0f, 50.0f, 0.0f, 1.0f},
            .Normal = FFixedPointVector4{0.0f, 0.0f, 1.0f, 0.0f},
            .Colour = FFixedPointVector4{0.0f, 0.0f, 1.0f, 1.0f}
        }
    };
    
    HelperPrepareMemory(IndexBuffer, VertexBuffer);

    const size_t kIndexBufferSize = IndexBuffer.size() * sizeof(uint32_t);
    HelperStartModule(0, kIndexBufferSize, 1);
    
    const int kMaxNumTicks = 100;
    EXPECT_TRUE(HelperTickModuleUntilValidOrReady(kMaxNumTicks));

    // state = READY
    ASSERT_EQ(0, Module.o_valid);
    ASSERT_EQ(1, Module.o_ready);
    ASSERT_EQ(1, Module.o_debug_triangle_index);
}

TEST_F(TriangleAssembly, ShouldWaitUntilRasterizerIsReadyToRenderSingleTriangle)
{
    HelperResetTestBench();

    auto& Module = TestBench.Module;

    // prepare a triangle for render
    std::vector<uint32_t> IndexBuffer = {0, 1, 2};
    std::vector<FFixedPointVertex> VertexBuffer = {
        {
            .Position = FFixedPointVector4(10.0f, 0.0f, 0.0f, 1.0f),
            .Normal = FFixedPointVector4(0.0f, 0.0f, 1.0f, 0.0f),
            .Colour = FFixedPointVector4(1.0f, 0.0f, 0.0f, 1.0f)
        },
        {
            .Position = FFixedPointVector4{100.0f, 0.0f, 0.0f, 1.0f},
            .Normal = FFixedPointVector4{0.0f, 0.0f, 1.0f, 0.0f},
            .Colour = FFixedPointVector4{0.0f, 1.0f, 0.0f, 1.0f}
        },
        {
            .Position = FFixedPointVector4{50.0f, 50.0f, 0.0f, 1.0f},
            .Normal = FFixedPointVector4{0.0f, 0.0f, 1.0f, 0.0f},
            .Colour = FFixedPointVector4{0.0f, 0.0f, 1.0f, 1.0f}
        }
    };

    HelperPrepareMemory(IndexBuffer, VertexBuffer);

    const uint32_t kIndexBufferSize = IndexBuffer.size() * sizeof(uint32_t);
    HelperStartModule(0, kIndexBufferSize, 1);

    // block triangle assembly
    Module.i_rasterizer_ready = 0;
    
    const int kMaxNumTicks = 100;
    EXPECT_FALSE(HelperTickModuleUntilValidOrReady(kMaxNumTicks));

    // state = READY_TO_RASTERIZE
    ASSERT_EQ(0, Module.o_valid);
    ASSERT_EQ(0, Module.o_ready);

    Module.i_rasterizer_ready = 1;
    TestBench.Tick();

    // state = START_RASTERIZE
    ASSERT_EQ(1, Module.o_valid);
    ASSERT_EQ(0, Module.o_ready);

    TestBench.Tick();

    // state = TRIANGLE_COMPLETE
    ASSERT_EQ(0, Module.o_valid);
    ASSERT_EQ(0, Module.o_ready);

    TestBench.Tick();

    // state = READY
    ASSERT_EQ(0, Module.o_valid);
    ASSERT_EQ(1, Module.o_ready);
    ASSERT_EQ(1, Module.o_debug_triangle_index);
}

TEST_F(TriangleAssembly, ShouldRenderMultipleTriangles)
{
    HelperResetTestBench();

    auto& Module = TestBench.Module;

    // prepare a triangle for render
    std::vector<uint32_t> IndexBuffer = {0, 1, 2,
                                         2, 1, 3};
    std::vector<FFixedPointVertex> VertexBuffer = {
        {
            .Position = FFixedPointVector4(10.0f, 0.0f, 0.0f, 1.0f),
            .Normal = FFixedPointVector4(0.0f, 0.0f, 1.0f, 0.0f),
            .Colour = FFixedPointVector4(1.0f, 0.0f, 0.0f, 1.0f)
        },
        {
            .Position = FFixedPointVector4{100.0f, 0.0f, 0.0f, 1.0f},
            .Normal = FFixedPointVector4{0.0f, 0.0f, 1.0f, 0.0f},
            .Colour = FFixedPointVector4{0.0f, 1.0f, 0.0f, 1.0f}
        },
        {
            .Position = FFixedPointVector4{50.0f, 50.0f, 0.0f, 1.0f},
            .Normal = FFixedPointVector4{0.0f, 0.0f, 1.0f, 0.0f},
            .Colour = FFixedPointVector4{0.0f, 0.0f, 1.0f, 1.0f}
        },
        {
            .Position = FFixedPointVector4{100.0f, 100.0f, 0.0f, 1.0f},
            .Normal = FFixedPointVector4{0.0f, 0.0f, 1.0f, 0.0f},
            .Colour = FFixedPointVector4{1.0f, 1.0f, 0.0f, 1.0f}
        }
    };

    HelperPrepareMemory(IndexBuffer, VertexBuffer);

    const uint32_t kIndexBufferSize = IndexBuffer.size() * sizeof(uint32_t);
    HelperStartModule(0, kIndexBufferSize, 2);
    
    const int kMaxNumTicks = 100;
    
    // Triangle 1
    EXPECT_TRUE(HelperTickModuleUntilValidOrReady(kMaxNumTicks));

    // state = START_RASTERIZER
    ASSERT_EQ(1, Module.o_valid);
    ASSERT_EQ(0, Module.o_ready);

    TestBench.Tick();

    // Triangle 2
    EXPECT_TRUE(HelperTickModuleUntilValidOrReady(kMaxNumTicks));

    // state = START_RASTERIZER
    ASSERT_EQ(1, Module.o_valid);
    ASSERT_EQ(0, Module.o_ready);

    TestBench.Tick();

    // state = FINISHED_TRIANGLE
    ASSERT_EQ(0, Module.o_valid);
    ASSERT_EQ(0, Module.o_ready);

    TestBench.Tick();

    // state = READY
    ASSERT_EQ(0, Module.o_valid);
    ASSERT_EQ(1, Module.o_ready);
    ASSERT_EQ(2, Module.o_debug_triangle_index);
}