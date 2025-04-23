`include "Matrix44.vh"

/**
 * TriangleAssembly
 *
 * Assemble a sequence of triangles from a 
 */
module TriangleAssembly(
    input i_clk,
    input i_reset_n,

    input i_start,                          // raise for one clock cyle, to start rendering triangles described by i_vertex_buffer, i_index_buffer, and i_num_triangles
    input [31:0] i_address_index_buffer,    // array of uint16 - memory should remain valid until o_ready is raised
    input [31:0] i_address_vertex_buffer,   // array of Vertex_t (local space) - memory should remain valid until o_ready is raised
    input [31:0] i_num_triangles,           // number of triangles - should be valid when i_start is raised
    input Matrix44_t i_world,               // world transform  - should be valid when i_start is raised
    input Matrix44_t i_view_projection,     // concatenated view + projection matrices - should be valid when i_start is raised
    input `FixedPoint_t i_screenWidth,      // width of screen that we are rendering to
    input `FixedPoint_t i_screenHeight,     // height of screen that we are rendering to

    input Vector4_t i_light_direction,      // direction that directional light is pointing in

    input i_rasterizer_ready,               // raise this input to pause the TriangleAssembly pipeline (i.e. if all Triangle Rasterizers are busy)
                                            // The pipeline will pause at the state of outputting a triangle
    
    output reg o_ready,                     // raised while the TriangleAssembly pipeline is ready
                                            // to start a new request
    
    output reg o_valid,                     // raised when a triangle is being output on o_v1, o_v2, o_v3, o_c1, o_c2, o_c3
    output Vector4_t o_v1, o_v2, o_v3,      // ScreenSpace Triangle Vertex locations
    output Vector4_t o_c1, o_c2, o_c3,      // Triangle Vertex colours

    output reg [31:0] o_memory_address,     // memory address to read from
    output reg o_memory_read,               // raised when assembly wants to read from memory at o_address
    input [31:0] i_memory_data,             // 32 bits of data ready from memory at o_address

    output reg o_error,                     // an error has occurred

    output reg [3:0] o_debug_state,
    output reg [31:0] o_debug_state_counter,
    output reg [31:0] o_debug_triangle_index,
    output reg [31:0] o_debug_num_triangles
);

// data loaded from module API on i_start
reg [31:0] r_address_vertex_buffer;
reg [31:0] r_address_index_buffer;
reg [31:0] r_num_triangles;
Matrix44_t r_world;
Matrix44_t r_view_projection;
reg `FixedPoint_t r_screenWidth;
reg `FixedPoint_t r_screenHeight;
Vector4_t r_light_direction;

// internal state data
typedef enum logic [3:0] {
    READY,                                   // waiting for i_start to be raised
    LOAD_INDEX_BUFFER,                      // load 3 x indexes from index buffer
    LOAD_VERTEX_BUFFER,                     // load 3 x vertices (local space) from vertex buffer
                                            //  - Position
                                            //  - Normal (TODO)
                                            //  - Colour
    TRANSFORM_WORLD,                        // apply world transform to vertices (local space) to get vertices (world space)
    LIGHT,                                  // modify vertex colours (world space)
    TRANSFORM_VIEW_PROJECTION,              // apply view&projection to vertices (world space) to get vertices (screen space)
    BACKFACE_CULL,                          // apply backface culling to vertices (screen space)
    READY_TO_RASTERIZE,                     // ready to rasterize a triangle, when a rasterizer is available
    START_RASTERIZE,                        // start rasterizing a triangle (vertices (screen space) + vertex colours)
    FINISHED_TRIANGLE,                      // finished assembling a triangle
    ERROR                                   // an error has occurred
} State_t;

State_t r_state;                            // current state
reg [31:0] r_state_counter;                 // counter, used within each state

reg [31:0] r_triangle_index;                // index into sequence of triangles

reg [31:0] r_i1, r_i2, r_i3;                // Vertex Indices
Vector4_t r_v1, r_v2, r_v3;                 // Vertex positions
Vector4_t r_n1, r_n2, r_n3;                 // Normal positions
Vector4_t r_c1, r_c2, r_c3;                 // Vertex colours

function [31:0] memory_address_index_buffer;
    input [31:0] triangle_index;
    input [31:0] vertex_index;

    begin
        memory_address_index_buffer = r_address_index_buffer + (((3 * triangle_index) + vertex_index) * 4);
    end
endfunction

// generate memory address for a component (x,y,z,w) of the position field of a vertex
function [31:0] memory_address_vertex_buffer_position;
    input [31:0] vertex_index;
    input [31:0] component_index;

    begin
        memory_address_vertex_buffer_position = r_address_vertex_buffer + (((12 * vertex_index) + component_index) * 4);
    end
endfunction

// generate memory address for a component (x,y,z,w) of the normal field of a vertex
function [31:0] memory_address_vertex_buffer_normal;
    input [31:0] vertex_index;
    input [31:0] component_index;

    begin
        memory_address_vertex_buffer_normal = r_address_vertex_buffer + (((12 * vertex_index) + 4 + component_index) * 4);
    end
endfunction

// generate memory address for a component (x,y,z,w) of the colour field of a vertex
function [31:0] memory_address_vertex_buffer_colour;
    input [31:0] vertex_index;
    input [31:0] component_index;

    begin
        memory_address_vertex_buffer_colour = r_address_vertex_buffer + (((12 * vertex_index) + 8 + component_index) * 4);
    end
endfunction

// View Projection hardware

Vector4_t r_view_project_v1;
Vector4_t r_view_project_v2;
Vector4_t r_view_project_v3;

VertexTransform view_project_v1(
    .i_vertex(r_v1),
    .i_matrix(r_view_projection),
    .i_screenWidth(r_screenWidth),
    .i_screenHeight(r_screenHeight),
    .o_vertex(r_view_project_v1)
);

VertexTransform view_project_v2(
    .i_vertex(r_v2),
    .i_matrix(r_view_projection),
    .i_screenWidth(r_screenWidth),
    .i_screenHeight(r_screenHeight),
    .o_vertex(r_view_project_v2)
);

VertexTransform view_project_v3(
    .i_vertex(r_v3),
    .i_matrix(r_view_projection),
    .i_screenWidth(r_screenWidth),
    .i_screenHeight(r_screenHeight),
    .o_vertex(r_view_project_v3)
);

// Lighting

Vector4_t r_lit_c1;
Vector4_t r_lit_c2;
Vector4_t r_lit_c3;

Lighting lighting_v1(
    .i_colour(r_c1),
    .i_normal(r_n1),
    .i_light_direction(r_light_direction),
    .o_colour(r_lit_c1)
);

Lighting lighting_v2(
    .i_colour(r_c2),
    .i_normal(r_n2),
    .i_light_direction(r_light_direction),
    .o_colour(r_lit_c2)
);

Lighting lighting_v3(
    .i_colour(r_c3),
    .i_normal(r_n3),
    .i_light_direction(r_light_direction),
    .o_colour(r_lit_c3)
);

// Back face culling
reg r_cull;

TriangleBackFaceCull back_face_cull(
    .i_v1(r_v1),
    .i_v2(r_v2),
    .i_v3(r_v3),
    .o_result(r_cull)
);

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
    begin
        ////////////////////////////////////////////////////////////////
        // reset
        ////////////////////////////////////////////////////////////////
        
        r_state <= READY;
        r_state_counter <= 0;

        r_triangle_index <= 0;

        o_memory_read <= 0;
        o_memory_address <= 0;

    end else if (i_start)
    begin
        ////////////////////////////////////////////////////////////////
        // start processing a sequence of triangles
        ////////////////////////////////////////////////////////////////

        // load data from module's API        
        r_address_vertex_buffer <= i_address_vertex_buffer;
        r_address_index_buffer <= i_address_index_buffer;
        r_num_triangles <= i_num_triangles;
        r_world <= i_world;
        r_view_projection <= i_view_projection;
        r_screenWidth <= i_screenWidth;
        r_screenHeight <= i_screenHeight;
        r_light_direction <= i_light_direction;

        // initialise state
        r_state <= LOAD_INDEX_BUFFER;
        r_state_counter <= 0;
        r_triangle_index <= 0;

    end else
    begin
        ////////////////////////////////////////////////////////////////
        // process state machine
        ////////////////////////////////////////////////////////////////

        o_memory_address <= 0;
        o_memory_read <= 0;

        case (r_state)
            READY: begin
            end
            LOAD_INDEX_BUFFER: begin
                // load 3 x indices into local registers

                r_state_counter <= r_state_counter + 1;

                case (r_state_counter)
                0: begin
                    o_memory_read <= 1;
                    o_memory_address <= memory_address_index_buffer(r_triangle_index, 0);
                end
                1: begin
                    r_i1 <= i_memory_data;

                    o_memory_read <= 1;
                    o_memory_address <= memory_address_index_buffer(r_triangle_index, 1);
                end
                2: begin
                    r_i2 <= i_memory_data;

                    o_memory_read <= 1;
                    o_memory_address <= memory_address_index_buffer(r_triangle_index, 2);
                end 
                3: begin
                    r_i3 <= i_memory_data;

                    r_state <= LOAD_VERTEX_BUFFER;
                    r_state_counter <= 0;
                end
                endcase
            end
            LOAD_VERTEX_BUFFER: begin
                // load 3 x vertices (position + normal + colour) into local registers

                o_memory_read <= 1;
                r_state_counter <= r_state_counter + 1;

                case (r_state_counter)
                0: begin
                    o_memory_address <= memory_address_vertex_buffer_position(r_i1, 0);
                end
                1: begin
                    r_v1.x <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i1, 1);
                end
                2: begin
                    r_v1.y <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i1, 2);
                end
                3: begin
                    r_v1.z <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i1, 3);
                end
                4: begin
                    r_v1.w <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i2, 0);
                end
                5: begin
                    r_v2.x <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i2, 1);
                end
                6: begin
                    r_v2.y <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i2, 2);
                end
                7: begin
                    r_v2.z <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i2, 3);
                end
                8: begin
                    r_v2.w <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i3, 0);
                end
                9: begin
                    r_v3.x <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i3, 1);
                end
                10: begin
                    r_v3.y <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i3, 2);
                end
                11: begin
                    r_v3.z <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_position(r_i3, 3);
                end
                12: begin
                    r_v3.w <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i1, 0);
                end
                13: begin
                    r_n1.x <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i1, 1);
                end
                14: begin
                    r_n1.y <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i1, 2);
                end
                15: begin
                    r_n1.z <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i1, 3);
                end
                16: begin
                    r_n1.w <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i2, 0);
                end
                17: begin
                    r_n2.x <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i2, 1);
                end
                18: begin
                    r_n2.y <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i2, 2);
                end
                19: begin
                    r_n2.z <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i2, 3);
                end
                20: begin
                    r_n2.w <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i3, 0);
                end
                21: begin
                    r_n3.x <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i3, 1);
                end
                22: begin
                    r_n3.y <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i3, 2);
                end
                23: begin
                    r_n3.z <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_normal(r_i3, 3);
                end
                24: begin
                    r_n3.w <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i1, 0);
                end
                25: begin
                    r_c1.x <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i1, 1);
                end
                26: begin
                    r_c1.y <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i1, 2);
                end
                27: begin
                    r_c1.z <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i1, 3);
                end
                28: begin
                    r_c1.w <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i2, 0);
                end
                29: begin
                    r_c2.x <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i2, 1);
                end
                30: begin
                    r_c2.y <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i2, 2);
                end
                31: begin
                    r_c2.z <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i2, 3);
                end
                32: begin
                    r_c2.w <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i3, 0);
                end
                33: begin
                    r_c3.x <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i3, 1);
                end
                34: begin
                    r_c3.y <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i3, 2);
                end
                35: begin
                    r_c3.z <= i_memory_data;
                    o_memory_address <= memory_address_vertex_buffer_colour(r_i3, 3);
                end
                36: begin
                    r_c3.w <= i_memory_data;
                    o_memory_read <= 0;
                    
                    r_state <= TRANSFORM_WORLD;
                    r_state_counter <= 0;
                end
                endcase
            end
            TRANSFORM_WORLD: begin
                // transform 3 x vertices from local space to world space

                r_v1 <= matrix_multiply_vector(r_world, r_v1);
                r_v2 <= matrix_multiply_vector(r_world, r_v2);
                r_v3 <= matrix_multiply_vector(r_world, r_v3);

                r_n1 <= matrix_multiply_vector(r_world, r_n1);
                r_n2 <= matrix_multiply_vector(r_world, r_n2);
                r_n3 <= matrix_multiply_vector(r_world, r_n3);

                r_state <= LIGHT;
            end
            LIGHT: begin
                // light 3 x vertices in world space

                r_c1 <= r_lit_c1;
                r_c2 <= r_lit_c2;
                r_c3 <= r_lit_c3;

                r_state <= TRANSFORM_VIEW_PROJECTION;

            end
            TRANSFORM_VIEW_PROJECTION: begin
                // transform 3 x vertices from world space to screen space
                r_v1 <= r_view_project_v1;
                r_v2 <= r_view_project_v2;
                r_v3 <= r_view_project_v3;

                r_state <= BACKFACE_CULL;

            end
            BACKFACE_CULL: begin
                // decide whether to backface cull this triangle, or rasterize it
                if (r_cull)
                begin
                    r_state <= FINISHED_TRIANGLE;
                end
                else
                begin
                    r_state <= READY_TO_RASTERIZE;
                end
            end
            READY_TO_RASTERIZE: begin
                // wait until a rasterizer is available
                if (i_rasterizer_ready)
                begin
                    r_state <= START_RASTERIZE;
                end
            end
            START_RASTERIZE: begin
                // output the current screenspace triangle to the rasterizer
                // (see synchronous always for 'o_valid' logic)

                // prepare to finish processing the current triangle
                r_state <= FINISHED_TRIANGLE;                
            end
            FINISHED_TRIANGLE: begin
                r_triangle_index <= r_triangle_index + 1;

                if ((r_triangle_index + 1) >= r_num_triangles)
                begin
                    // finished assembling all triangles
                    r_state <= READY;
                end else begin
                    // start assembling the next triangle    
                    r_state <= LOAD_INDEX_BUFFER;
                    r_state_counter <= 0;
                end
            end
            ERROR: begin
            end
            default: begin
                // invalid state
                r_state <= ERROR;
            end
        endcase

    end
end

always @(*)
begin
    o_ready = 0;
    o_valid = 0;
    o_error = 0;

    o_v1 = r_v1;
    o_v2 = r_v2;
    o_v3 = r_v3;
    o_c1 = r_c1;
    o_c2 = r_c2;
    o_c3 = r_c3;

    o_debug_state = r_state;
    o_debug_state_counter = r_state_counter;
    o_debug_triangle_index = r_triangle_index;
    o_debug_num_triangles = r_num_triangles;

    case (r_state)
        READY: begin
            o_ready = 1;
        end
        START_RASTERIZE: begin
            o_valid = 1;
        end
        ERROR: begin
            o_error = 1;
        end
        default: begin
        end
    endcase
end

endmodule