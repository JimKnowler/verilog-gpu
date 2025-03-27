module TriangleRasterizer (
    input [31:0] x,
    input [31:0] y,

    input [31:0] p1x,
    input [31:0] p1y,
    input [31:0] p2x,
    input [31:0] p2y,
    input [31:0] p3x,
    input [31:0] p3y,

    output reg [31:0] r,
    output reg [31:0] g,
    output reg [31:0] b
);

always @(*)
begin
    r = 255;
    g = 0;
    b = 0;
end

endmodule 
