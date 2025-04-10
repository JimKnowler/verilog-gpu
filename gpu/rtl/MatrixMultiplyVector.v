/**
 * Apply a Matrix to a Vector
 */

`include "FixedPoint.vh"

/**
 * 4d Vector
 */
typedef struct packed {
    logic signed [`FIXEDPOINT_WIDTH-1:0] x, y, z, w;
} Vector4_t;

/**
 * 4x4 Matrix
 * Row-Major storage - can easily multiply vectors with dot-product
 */ 
typedef struct packed {
    Vector4_t [3:0] rows;
} Matrix44_t;

function signed [`FIXEDPOINT_WIDTH-1:0] vector_dot_product;
    input Vector4_t a, b;

    begin
        vector_dot_product = fixed_point_multiply(a.x, b.x) + fixed_point_multiply(a.y, b.y) + fixed_point_multiply(a.z, b.z) + fixed_point_multiply(a.w, b.w);
    end
endfunction

function Vector4_t matrix_multiply_vector;
    input Matrix44_t m;
    input Vector4_t v;

    begin
        matrix_multiply_vector.x = vector_dot_product(m.rows[0], v);
        matrix_multiply_vector.y = vector_dot_product(m.rows[1], v);
        matrix_multiply_vector.z = vector_dot_product(m.rows[2], v);
        matrix_multiply_vector.w = vector_dot_product(m.rows[3], v);
    end
endfunction

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
