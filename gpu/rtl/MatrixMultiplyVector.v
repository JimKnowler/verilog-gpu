/**
 * Apply a Matrix to a Vector
 */

`include "Matrix44.vh"

module MatrixMultiplyVector ( 
    input Matrix44_t i_matrix,
    input Vector4_t i_vector,
    output Vector4_t o_vector
);
    always @(*)
    begin
        o_vector = matrix_multiply_vector(i_matrix, i_vector);
    end
endmodule
