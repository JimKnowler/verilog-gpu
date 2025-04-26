
`include "FixedPoint.vh"
`include "Vector4.vh"

module TriangleRasterizerPixel (
    // FixedPoint: x,y screen location of rasterized pixel
    input signed [31:0] i_x, i_y,

    // ScreenSpace locations for vertices
    input Vector4_t i_v1, i_v2, i_v3,

    // RGBA colours for vertices
    input Vector4_t i_c1, i_c2, i_c3,

    // RGBA colour output of rasterized pixel
    output Vector4_t o_colour,

    // FixedPoint: z co-ordinate of rasterized pixel
    output reg `FixedPoint_t o_z,

    // signals when a pixel inside the triangle is being written
    output reg o_write
);

/*
 * function: edge_function
 * 
 * determine on which side of edge (a->b) point (p) is
 *   w > 0 : p is on left side of the edge
 *   w < 0 : p is on right side of the edge
 *   w == 0 : p is exactly on the edge
 */
function `FixedPoint_t edge_function;
    // ax, ay = edge start point
    // bx, by = edge end point
    // px, py = point being tested
    input `FixedPoint_t ax, ay, bx, by, px, py;
    begin
        edge_function = fixed_point_sub(fixed_point_multiply(fixed_point_sub(bx, ax), fixed_point_sub(py, ay)), fixed_point_multiply(fixed_point_sub(by, ay), fixed_point_sub(px, ax)));
    end
endfunction

reg `FixedPoint_t r_x;
reg `FixedPoint_t r_y;
reg `FixedPoint_t r_w1;
reg `FixedPoint_t r_w2;
reg `FixedPoint_t r_w3;
reg `FixedPoint_t r_area;
reg `FixedPoint_t r_w1_norm;
reg `FixedPoint_t r_w2_norm;
reg `FixedPoint_t r_w3_norm;

reg `FixedPoint_t r_z_inv;
reg `FixedPoint_t r_v1_z_inv;
reg `FixedPoint_t r_v2_z_inv;
reg `FixedPoint_t r_v3_z_inv;

always @(*)
begin
    r_x = int32_to_fixed_point(i_x);
    r_y = int32_to_fixed_point(i_y);
    r_w1 = edge_function(i_v2.x, i_v2.y, i_v3.x, i_v3.y, r_x, r_y);
    r_w2 = edge_function(i_v3.x, i_v3.y, i_v1.x, i_v1.y, r_x, r_y);
    r_w3 = edge_function(i_v1.x, i_v1.y, i_v2.x, i_v2.y, r_x, r_y);

    r_z_inv = 0;
    r_v1_z_inv = fixed_point_divide(uint8_to_fixed_point(1), i_v1.z);
    r_v2_z_inv = fixed_point_divide(uint8_to_fixed_point(1), i_v2.z);
    r_v3_z_inv = fixed_point_divide(uint8_to_fixed_point(1), i_v3.z);

    r_area = edge_function(i_v1.x, i_v1.y, i_v2.x, i_v2.y, i_v3.x, i_v3.y);

    r_w1_norm = fixed_point_divide(r_w1, r_area);
    r_w2_norm = fixed_point_divide(r_w2, r_area);
    r_w3_norm = fixed_point_divide(r_w3, r_area);

    o_colour.x = 0;
    o_colour.y = 0;
    o_colour.z = 0;
    o_colour.w = 0;
    o_z = 0;
    o_write = 0;

    if ((r_w1 >= 0) && (r_w2 >= 0) && (r_w3 >= 0))
    begin
        // interpolate colour
        o_colour.x = fixed_point_multiply(r_w1_norm, i_c1.x) + fixed_point_multiply(r_w2_norm, i_c2.x) + fixed_point_multiply(r_w3_norm, i_c3.x);
        o_colour.y = fixed_point_multiply(r_w1_norm, i_c1.y) + fixed_point_multiply(r_w2_norm, i_c2.y) + fixed_point_multiply(r_w3_norm, i_c3.y);
        o_colour.z = fixed_point_multiply(r_w1_norm, i_c1.z) + fixed_point_multiply(r_w2_norm, i_c2.z) + fixed_point_multiply(r_w3_norm, i_c3.z);
        o_colour.w = uint8_to_fixed_point(1);

        // interpolate 1/z
        r_z_inv = fixed_point_multiply(r_w1_norm, r_v1_z_inv) + fixed_point_multiply(r_w2_norm, r_v2_z_inv) + fixed_point_multiply(r_w3_norm, r_v3_z_inv);
        o_z = fixed_point_divide(uint8_to_fixed_point(1), r_z_inv);

        o_write = 1;
    end
end

endmodule 
