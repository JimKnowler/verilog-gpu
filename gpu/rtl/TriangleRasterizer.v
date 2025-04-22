`include "FixedPoint.vh"
`include "Vector4.vh"

module TriangleRasterizer (
    input i_clk,
    input i_reset_n,
    
    // raise (for one cycle) to start rasterizing a new triangle
    input i_start,    
    input Vector4_t i_v1, i_v2, i_v3,
    input Vector4_t i_c1, i_c2, i_c3,

    output reg o_write,
    output Vector4_t o_colour,
    output reg signed [31:0] o_x,
    output reg signed [31:0] o_y,

    // signal (continously) that the rasterizer is ready to start rasterizing a new triangle
    output reg o_ready
);

wire w_valid;
wire w_write;
wire [31:0] w_x;
wire [31:0] w_y;
Vector4_t r_v1, r_v2, r_v3;
Vector4_t r_c1, r_c2, r_c3;

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
    begin
        r_v1 <= 0;
        r_v2 <= 0;
        r_v3 <= 0;
        r_c1 <= 0;
        r_c2 <= 0;
        r_c3 <= 0;
    end else if (i_start) begin
        r_v1 <= i_v1;
        r_v2 <= i_v2;
        r_v3 <= i_v3;
        r_c1 <= i_c1;
        r_c2 <= i_c2;
        r_c3 <= i_c3;
    end
end

TriangleRasterizerTiming timing(
    .i_clk(i_clk),
    .i_reset_n(i_reset_n),
    .i_v1(i_v1),
    .i_v2(i_v2),
    .i_v3(i_v3),
    .i_start(i_start),
    .o_x(w_x),
    .o_y(w_y),
    .o_ready(o_ready),
    .o_valid(w_valid)
);

TriangleRasterizerPixel pixel(
    .i_x(w_x),
    .i_y(w_y),
    .i_v1(r_v1),
    .i_v2(r_v2),
    .i_v3(r_v3),
    .i_c1(r_c1),
    .i_c2(r_c2),
    .i_c3(r_c3),
    .o_colour(o_colour),
    .o_write(w_write)
);

assign o_write = w_valid && w_write;
assign o_x = w_x;
assign o_y = w_y;

endmodule
