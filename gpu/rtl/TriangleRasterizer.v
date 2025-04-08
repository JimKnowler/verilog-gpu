`include "FixedPoint.vh"

module TriangleRasterizer (
    // FixedPoint x,y screen location of rasterized pixel
    // TODO: can we render fractions of screen space pixels?  or should this be plain integer (and convert to fixed point internally)
    input signed [`FIXEDPOINT_WIDTH-1:0] i_x, i_y,

    // FixedPoint x,y screen location & colour of triangle vertices
    input signed [`FIXEDPOINT_WIDTH-1:0] i_v1x, i_v1y, i_v1r, i_v1g, i_v1b,
    input signed [`FIXEDPOINT_WIDTH-1:0] i_v2x, i_v2y, i_v2r, i_v2g, i_v2b,
    input signed [`FIXEDPOINT_WIDTH-1:0] i_v3x, i_v3y, i_v3r, i_v3g, i_v3b,

    // Integer r,g,b colour output of rasterized pixel
    output reg [7:0] o_r, o_g, o_b
);

/*
 * function: edge_function
 * 
 * determine on which side of edge (a->b) point (p) is
 *   w > 0 : p is on left side of the edge
 *   w < 0 : p is on right side of the edge
 *   w == 0 : p is exactly on the edge
 */
function signed [`FIXEDPOINT_WIDTH-1:0] edge_function;
    // ax, ay = edge start point
    // bx, by = edge end point
    // px, py = point being tested
    input [`FIXEDPOINT_WIDTH-1:0] ax, ay, bx, by, px, py;
    begin
        edge_function = fixed_point_sub(fixed_point_multiply(fixed_point_sub(bx, ax), fixed_point_sub(py, ay)), fixed_point_multiply(fixed_point_sub(by, ay), fixed_point_sub(px, ax)));
    end
endfunction

// TODO: precompute edge function coefficients for triangle
// -> ready for the form EdgeFunction = Ax + By + C

// TODO: barycentric co-ords to interpolate colour across the surface of the triangle
//   -> Precompute denominator E(A, B, C)

reg signed [`FIXEDPOINT_WIDTH-1:0] r_w1;
reg signed [`FIXEDPOINT_WIDTH-1:0] r_w2;
reg signed [`FIXEDPOINT_WIDTH-1:0] r_w3;
reg signed [`FIXEDPOINT_WIDTH-1:0] r_area;
reg signed [`FIXEDPOINT_WIDTH-1:0] r_w1_norm;
reg signed [`FIXEDPOINT_WIDTH-1:0] r_w2_norm;
reg signed [`FIXEDPOINT_WIDTH-1:0] r_w3_norm;

always @(*)
begin
    r_w1 = edge_function(i_v1x, i_v1y, i_v2x, i_v2y, i_x, i_y);
    r_w2 = edge_function(i_v2x, i_v2y, i_v3x, i_v3y, i_x, i_y);
    r_w3 = edge_function(i_v3x, i_v3y, i_v1x, i_v1y, i_x, i_y);

    r_area = edge_function(i_v1x, i_v1y, i_v2x, i_v2y, i_v3x, i_v3y);

    r_w1_norm = fixed_point_divide(r_w1, r_area);
    r_w2_norm = fixed_point_divide(r_w2, r_area);
    r_w3_norm = fixed_point_divide(r_w3, r_area);

    o_r = 0;
    o_g = 0;
    o_b = 0;

    if ((r_w1 >= 0) && (r_w2 >= 0) && (r_w3 >= 0))
    begin
        o_r = fixed_point_to_uint8(fixed_point_multiply(uint8_to_fixed_point(255), fixed_point_multiply(r_w1_norm, i_v1r) + fixed_point_multiply(r_w2_norm, i_v2r) + fixed_point_multiply(r_w3_norm, i_v3r)));
        o_g = fixed_point_to_uint8(fixed_point_multiply(uint8_to_fixed_point(255), fixed_point_multiply(r_w1_norm, i_v1g) + fixed_point_multiply(r_w2_norm, i_v2g) + fixed_point_multiply(r_w3_norm, i_v3g)));
        o_b = fixed_point_to_uint8(fixed_point_multiply(uint8_to_fixed_point(255), fixed_point_multiply(r_w1_norm, i_v1b) + fixed_point_multiply(r_w2_norm, i_v2b) + fixed_point_multiply(r_w3_norm, i_v3b)));
    end
end

endmodule 
