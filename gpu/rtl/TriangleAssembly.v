`include "Matrix44.vh"

module TriangleAssembly(
    i_clk,
    i_reset_n,

    input i_start,                      // raise for one clock cyle, to start rendering triangles described by i_vertex_buffer, i_index_buffer, and i_num_triangles
    input [31:0] i_vertex_buffer,       // array of Vertex_t - should remain valid until TriangleAssembly raised o_idle
    input [31:0] i_index_buffer,        // array of uint16 - should remain valid until TriangleAssembly raised o_idle
    input [31:0] i_num_triangles,       // number of triangles - should be valid when i_start is raised
    input Matrix44_t i_world,           // world transform  - should be valid when i_start is raised
    input Matrix44_t i_viewProjection,  // concatenated view + projection matrices - should be valid when i_start is raised

    input i_pause,                      // raise this input to pause the TriangleAssembly pipeline (i.e. if all Triangle Rasterizers are busy)
                                        // The pipeline will pause at the state of outputting a triangle
    
    output o_idle,                      // raised while the TriangleAssembly pipeline is idle.
    
    output o_valid,                     // raised when a triangle is being output on o_v1, o_v2, o_v3, o_c1, o_c2, o_c3
    output Vector4_t o_v1, o_v2, o_v3,  // ScreenSpace Triangle Vertex locations
    output Vector4_t o_c1, o_c2, o_c3   // Triangle Vertex colours

    output o_memory_address,            // memory address to read from
    output o_memory_read,               // raised when assembly wants to read from memory at o_address
    input [31:0] i_memory_data          // 32 bits of data ready from memory at o_address
);

// wait until i_start is valid
// register i_vertex_buffer, i_index_buffer, i_num_triangles, i_world, i_viewProjection

// for each triangle
// - read data for vertices (3 * 4 * 32bit) and colours (3 * 4 * 32bit)
// - apply world transform
// TODO - apply lighting at this stage
// - apply viewProjection transform
// - 

// i_pause - only use when pipeline is ready to output a triangle



endmodule