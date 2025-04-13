`include "FixedPoint.vh"

module FixedPointAdd (
    input `FixedPoint_t i_a,
    input `FixedPoint_t i_b,
    output reg `FixedPoint_t o_result
);

always @(*)
begin
    o_result = fixed_point_add(i_a, i_b);
end

endmodule
