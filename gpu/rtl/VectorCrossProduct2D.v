`include "Vector4.vh"

module VectorCrossProduct (
    input Vector4_t i_a,
    input Vector4_t i_b,
    output reg `FixedPoint_t o_result
);
    always @(*)
    begin
        o_result = vector_cross_product_2d(i_a, i_b);
    end
endmodule
