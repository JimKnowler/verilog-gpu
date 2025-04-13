`include "Vector4.vh"

module VectorDotProduct (
    input Vector4_t i_a,
    input Vector4_t i_b,
    output `FixedPoint_t o_result
);
    always @(*)
    begin
        o_result = vector_dot_product(i_a, i_b);
    end
endmodule
