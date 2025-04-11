`include "Vector4.vh"

module VectorDotProduct (
    input Vector4_t i_a,
    input Vector4_t i_b,
    output Vector4_t o_result
);
    always @(*)
    begin
        o_result = vector_add_vector(i_a, i_b);
    end
endmodule
