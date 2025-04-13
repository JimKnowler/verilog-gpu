`include "FixedPoint.vh"

/**
 * 4d Vector
 */
typedef struct packed {
    logic `FixedPoint_t x, y, z, w;
} Vector4_t;

/**
 * 4d Vector Dot Product
 */
function `FixedPoint_t vector_dot_product;
    input Vector4_t a, b;

    begin
        vector_dot_product = fixed_point_multiply(a.x, b.x) + fixed_point_multiply(a.y, b.y) + fixed_point_multiply(a.z, b.z) + fixed_point_multiply(a.w, b.w);
    end
endfunction

/**
 * 4d Vector Addition
 */
function Vector4_t vector_add_vector;
    input Vector4_t a, b;

    begin
        vector_add_vector.x = a.x + b.x;
        vector_add_vector.y = a.y + b.y;
        vector_add_vector.z = a.z + b.z;
        vector_add_vector.w = a.w + b.w;
    end
endfunction

/**
 * 4d Vector Multiply
 */
function Vector4_t vector_multiply_scalar;
    input Vector4_t a;
    input `FixedPoint_t scalar;

    begin
        vector_multiply_scalar.x = fixed_point_multiply(a.x, scalar);
        vector_multiply_scalar.y = fixed_point_multiply(a.y, scalar);
        vector_multiply_scalar.z = fixed_point_multiply(a.z, scalar);
        vector_multiply_scalar.w = fixed_point_multiply(a.w, scalar);
    end
endfunction

/**
 * 4d Vector Divide
 */
function Vector4_t vector_divide_scalar;
    input Vector4_t a;
    input `FixedPoint_t scalar;

    begin
        vector_divide_scalar.x = fixed_point_divide(a.x, scalar);
        vector_divide_scalar.y = fixed_point_divide(a.y, scalar);
        vector_divide_scalar.z = fixed_point_divide(a.z, scalar);
        vector_divide_scalar.w = fixed_point_divide(a.w, scalar);
    end
endfunction
