cmake_minimum_required(VERSION 3.31.6)
cmake_policy(SET CMP0074 NEW)

project(verilog-gpu CXX)

set(CMAKE_CXX_STANDARD 20)

# based on verilator documentation
# https://veripool.org/guide/latest/verilating.html#cmake

find_package(verilator HINTS $ENV{VERILATOR_ROOT})
if(NOT verilator_FOUND)
    message(
        FATAL_ERROR
        "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable"
    )
endif()

include_directories(.)

add_executable(Main 
    gpu/App.cpp
    gpu/Vector4.cpp
    gpu/Matrix44.cpp
    gpu/VerilatorHelpers.cpp
    gpu/FixedPointVector4.cpp
)

verilate(Main SOURCES gpu/rtl/TriangleAssembly.v INCLUDE_DIRS gpu/rtl)
verilate(Main SOURCES gpu/rtl/TriangleRasterizer.v INCLUDE_DIRS gpu/rtl)

# TODO: remove these modules, when TriangleAssembly is operational
verilate(Main SOURCES gpu/rtl/TriangleBackFaceCull.v INCLUDE_DIRS gpu/rtl)
verilate(Main SOURCES gpu/rtl/VertexTransform.v INCLUDE_DIRS gpu/rtl)


target_link_libraries(Main PRIVATE
    "-framework OpenGL"
    "-framework GLUT"
)

#
# GoogleTest
#

# download google test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.16.0/googletest-1.16.0.tar.gz
)

# prevent googletest from overriding our compiler/linker options
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

#
# Unit Tests
#

add_executable(TestRtl
    test/ToFixedPoint.test.cpp    
    test/FromFixedPoint.test.cpp    
    test/FixedPointAdd.test.cpp    
    test/FixedPointSub.test.cpp
    test/FixedPointMultiply.test.cpp
    test/FixedPointDivide.test.cpp
    test/MathTestHelpers.cpp
    test/MatrixMultiplyVector.test.cpp
    test/Matrix44.test.cpp
    test/TriangleRasterizerTiming.test.cpp
    test/TriangleBackFaceCull.test.cpp
    test/TriangleAssembly.test.cpp
    test/VectorDotProduct.test.cpp
    test/VectorAddVector.test.cpp
    test/VectorSubVector.test.cpp
    test/VectorMultiplyScalar.test.cpp
    test/VectorDivideScalar.test.cpp
    test/VectorSelect.test.cpp
    test/VectorCrossProduct.test.cpp
    test/VectorCrossProduct2D.test.cpp
    test/VertexTransform.test.cpp
    gpu/Matrix44.cpp
    gpu/Vector4.cpp
    gpu/FixedPointVector4.cpp
    gpu/VerilatorHelpers.cpp
)

verilate(TestRtl SOURCES gpu/rtl/FixedPointAdd.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/FixedPointSub.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/FixedPointMultiply.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/FixedPointDivide.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/MatrixMultiplyVector.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/VectorDotProduct.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/VectorCrossProduct.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/VectorCrossProduct2D.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/VectorAddVector.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/VectorSubVector.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/VectorMultiplyScalar.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/VectorDivideScalar.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/VertexTransform.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/VectorSelect.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/TriangleRasterizerTiming.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/TriangleBackFaceCull.v INCLUDE_DIRS gpu/rtl)
verilate(TestRtl SOURCES gpu/rtl/TriangleAssembly.v INCLUDE_DIRS gpu/rtl)

target_link_libraries(TestRtl PRIVATE gtest_main)

add_test(NAME TestRtl COMMAND TestRtl)