module TriangleRasterizer (
    // x,y screen location of rasterized pixel
    input [9:0] x,
    input [9:0] y,

    // x,y screen location of triangle vertices
    input [9:0] v1x,
    input [9:0] v1y,
    input [9:0] v2x,
    input [9:0] v2y,
    input [9:0] v3x,
    input [9:0] v3y,

    // r,g,b colour output of rasterized pixel
    output reg [7:0] r,
    output reg [7:0] g,
    output reg [7:0] b
);

// NOTE: the output of this function needs to be much wider than the input.
//       - add sign (to indicate which side of the edge the point is on)
//       - add two values, each calculated by multiplying two values
// TODO: how can I calculate the precise, required width?
// 
/* verilator lint_off WIDTHEXPAND */
function signed [20:0] edge_function;
    // ax, ay = edge start point
    // bx, by = edge end point
    // cx, cy = point being tested
    input [9:0] ax, ay, bx, by, cx, cy;
    begin
        edge_function = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
    end
endfunction
/* verilator lint_on WIDTHEXPAND */

reg signed [20:0] w1;
reg signed [20:0] w2;
reg signed [20:0] w3;

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
    end
end

endmodule 
