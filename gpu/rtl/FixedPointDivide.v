`include "FixedPoint.vh"

module FixedPointDivide (
    input `FixedPoint_t i_a,
    input `FixedPoint_t i_b,
    output reg `FixedPoint_t o_result
);

always @(*)
begin    
    o_result = fixed_point_divide(i_a, i_b);
end

endmodule
