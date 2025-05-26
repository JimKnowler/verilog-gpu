# Verilog-Gpu

FPGA/Verilog implementation of a basic GPU
- fixed pipeline triangle renderer
- texture sampler
- interpolation of vertex colour + normal across triangles
- aiming for feature set comparable to mid/late 90's Graphics Card https://www.techpowerup.com/gpu-specs/voodoo-graphics-4-mb.c3570

## Progress

### ~Simulation~
  - GPU now runs in simulation in MacOS test app

### Synthesis
 - WORK IN PROGRESS
 - Requires rewriting triangle rasterizer to remove/minimize divides & multiplies
 - Requires verilog implementation of fixed point divide

### Add more features
 - Texture Mapping
 - Improve accuracy of z buffer

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


