`include "FixedPoint.vh"

module FixedPointMultiply (
    input signed [`FIXEDPOINT_WIDTH-1:0] i_a,
    input signed [`FIXEDPOINT_WIDTH-1:0] i_b,
    output reg signed [`FIXEDPOINT_WIDTH-1:0] o_result
);

always @(*)
begin    
    o_result = fixed_point_multiply(i_a, i_b);
end

endmodule
