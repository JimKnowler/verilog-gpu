module UnsignedDivide #(
    parameter WIDTH = 16
) (
    input i_reset_n,
    input i_clk,

    input i_start,                      // high for one tick, when starting a division
    input [WIDTH-1:0] i_dividend,       // must be valid when i_start is high
    input [WIDTH-1:0] i_divisor,        // must be valid when i_start is high

    output o_ready,                     // high when ready to start a new division
    output o_valid,                     // high for one tick when calculation is complete
    output [WIDTH-1:0] o_quotient,      // valid while o_valid is high
    output [WIDTH-1:0] o_remainder      // valid while o_valid is high
);

endmodule
