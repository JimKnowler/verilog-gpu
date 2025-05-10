module UnsignedDivide #(
    parameter WIDTH = 16
) (
    input i_reset_n,
    input i_clk,

    input i_start,                      // high for one tick, when starting a division
    input [WIDTH-1:0] i_dividend,       // must be valid when i_start is high
    input [WIDTH-1:0] i_divisor,        // must be valid when i_start is high

    output reg o_ready,                     // high when ready to start a new division
    output reg o_valid,                     // high for one tick when calculation is complete
    output reg [WIDTH-1:0] o_quotient,      // valid while o_valid is high
    output reg [WIDTH-1:0] o_remainder      // valid while o_valid is high
);

reg [$clog2(WIDTH): 0] r_step;

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
    begin
        r_step <= 0;
    end else begin
        // TODO: process i_start to start a division
        // TODO: process steps for in-progress division
    end
end


always @(*)
begin
    o_ready = (r_step == 0);
    o_valid = 0;
    o_quotient = 0;
    o_remainder = 0;
end


endmodule
