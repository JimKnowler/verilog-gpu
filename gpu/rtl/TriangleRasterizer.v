`include "FixedPoint.vh"
`include "Vector4.vh"

module TriangleRasterizer (
    input i_clk,
    input i_reset_n,
    
    input i_start,    
    input Vector4_t i_v1, i_v2, i_v3,
    input Vector4_t i_c1, i_c2, i_c3,

    output reg o_write,
    output Vector4_t o_colour,
    output reg signed [31:0] o_x,
    output reg signed [31:0] o_y,

    output reg o_idle
);

wire w_valid;
wire w_write;
wire [31:0] w_x;
wire [31:0] w_y;

TriangleRasterizerTiming timing(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),
    .i_v1(i_v1),
    .i_v2(i_v2),
    .i_v3(i_v3),
    .i_start(i_start),
    .o_x(w_x),
    .o_y(w_y),
    .o_idle(o_idle),
    .o_valid(w_valid)
);

// TODO: clock vertices and colours when i_start is high?

TriangleRasterizerPixel pixel(
    .i_x(w_x),
    .i_y(w_y),
    .i_v1(i_v1),
    .i_v2(i_v2),
    .i_v3(i_v3),
    .i_c1(i_c1),
    .i_c2(i_c2),
    .i_c3(i_c3),
    .o_colour(o_colour),
    .o_write(w_write)
);

assign o_write = w_valid && w_write;
assign o_x = w_x;
assign o_y = w_y;

endmodule
