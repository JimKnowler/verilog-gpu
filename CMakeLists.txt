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

add_executable(Main main/main.cpp gpu/App.cpp)
verilate(Main SOURCES gpu/rtl/Top.v)

target_link_libraries(Main PRIVATE
    "-framework OpenGL"
    "-framework GLUT"
)
