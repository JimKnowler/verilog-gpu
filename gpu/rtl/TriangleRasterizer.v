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

function signed [10:0] edge_function;
    input signed [9:0] ax, ay, bx, by, cx, cy;
    begin
        edge_function = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
    end
endfunction

reg signed [10:0] w0;
reg signed [10:0] w1;
reg signed [10:0] w2;

always @(*)
begin
    w0 = edge_function(v2x, v2y, v3x, v3y, x, y);
    w1 = edge_function(v3x, v3y, v1x, v1y, x, y);
    w2 = edge_function(v1x, v1y, v2x, v2y, x, y);

    r = 0;
    g = 0;
    b = 0;

    if ((w0 >= 0) && (w1 >= 0) && (w2 >= 0))
    begin
        r = 255;
    end
end

endmodule 
