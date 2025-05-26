# Verilog-Gpu

FPGA/Verilog implementation of a basic GPU
- fixed pipeline triangle renderer
- texture sampler
- interpolation of vertex colour + normal across triangles
- late 90's Graphics Card https://www.techpowerup.com/gpu-specs/voodoo-graphics-4-mb.c3570

## Progress

Working 

## Dependencies

Built on MacOS

Using Homebrew package manager
```
brew install verilator 
```

## Build

Build with CMake

Targets 
- TestRtl: Run unit tests on verilog designs
- Main: Run GPU in simulation


