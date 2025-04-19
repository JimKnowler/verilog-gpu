`include "Vector4.vh"

/**
 * 4x4 Matrix
 * Row-Major storage - can easily multiply vectors with dot-product
 */ 
typedef struct packed {
    Vector4_t [3:0] rows;
} Matrix44_t;

/**
 * Multiply Matrix with a Vector
 */
function Vector4_t matrix_multiply_vector;
    input Matrix44_t m;
    input Vector4_t v;

    begin
        // NOTE: reverse vector
        // - this was required after reversing order of x,y,z,w in Vector4_t
        Vector4_t r_v;
        r_v.x = v.w;
        r_v.y = v.z;
        r_v.z = v.y;
        r_v.w = v.x;

        // NOTE: row order is reversed here 
        //       (verilog rows are specified as [3:0])
        matrix_multiply_vector.x = vector_dot_product(m.rows[3], r_v);
        matrix_multiply_vector.y = vector_dot_product(m.rows[2], r_v);
        matrix_multiply_vector.z = vector_dot_product(m.rows[1], r_v);
        matrix_multiply_vector.w = vector_dot_product(m.rows[0], r_v);
    end
endfunction
