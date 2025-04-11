`include "Vector4.vh"

module VectorDotProduct (
    input Vector4_t i_a,
    input Vector4_t i_b,
    output signed [`FIXEDPOINT_WIDTH-1:0] o_scalar
);
    always @(*)
    begin
        o_scalar = vector_dot_product(i_a, i_b);
    end
endmodule
