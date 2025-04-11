`include "Vector4.vh"

module VectorDivideScalar (
    input Vector4_t i_a,
    input signed [`FIXEDPOINT_WIDTH-1:0] i_scalar,
    output Vector4_t o_result
);
    always @(*)
    begin
        o_result = vector_divide_scalar(i_a, i_scalar);
    end
endmodule
