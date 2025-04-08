`include "FixedPoint.vh"

module TriangleRasterizer (
    // FixedPoint x,y screen location of rasterized pixel
    input signed [`FIXEDPOINT_WIDTH-1:0] x, y,

    // FixedPoint x,y screen location of triangle vertices
    input signed [`FIXEDPOINT_WIDTH-1:0] v1x, v1y, v2x, v2y, v3x, v3y,

    // Integer r,g,b colour output of rasterized pixel
    output reg [7:0] r, g, b
);

// determine on which side of edge (a->b) point (p) is
//   w > 0 : p is on left side of the edge
//   w < 0 : p is on right side of the edge
//   w == 0 : p is exactly on the edge
//
// NOTE: the output of this function needs to be much wider than the input.
//       - add sign (to indicate which side of the edge the point is on)
//       - add two values, each calculated by multiplying two values
// TODO: how can I calculate the precise, required bit width?
// 

`define EDGEFUNCTION_WIDTH `FIXEDPOINT_WIDTH*2

/* verilator lint_off WIDTHEXPAND */
function signed [`EDGEFUNCTION_WIDTH-1:0] edge_function;
    // ax, ay = edge start point
    // bx, by = edge end point
    // px, py = point being tested
    input [`FIXEDPOINT_WIDTH-1:0] ax, ay, bx, by, px, py;
    begin
        edge_function = fixed_point_sub(fixed_point_multiply(fixed_point_sub(bx, ax), fixed_point_sub(py, ay)), fixed_point_multiply(fixed_point_sub(by, ay), fixed_point_sub(px, ax)));
    end
endfunction
/* verilator lint_on WIDTHEXPAND */

// TODO: precompute edge function coefficients for triangle
// -> ready for the form EdgeFunction = Ax + By + C

// TODO: barycentric co-ords to interpolate colour across the surface of the triangle
//   -> Precompute denominator E(A, B, C)

reg signed [`EDGEFUNCTION_WIDTH-1:0] w1;
reg signed [`EDGEFUNCTION_WIDTH-1:0] w2;
reg signed [`EDGEFUNCTION_WIDTH-1:0] w3;

always @(*)
begin
    w1 = edge_function(v1x, v1y, v2x, v2y, x, y);
    w2 = edge_function(v2x, v2y, v3x, v3y, x, y);
    w3 = edge_function(v3x, v3y, v1x, v1y, x, y);
    
    r = 0;
    g = 0;
    b = 0;

    if ((w1 >= 0) && (w2 >= 0) && (w3 >= 0))
    begin
        r = 255;
        g = 255;
        b = 255;
    end
end

endmodule 
