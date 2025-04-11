`include "FixedPoint.vh"

/**
 * 4d Vector
 */
typedef struct packed {
    logic signed [`FIXEDPOINT_WIDTH-1:0] x, y, z, w;
} Vector4_t;

/**
 * 4d Vector Dot Product
 */
function signed [`FIXEDPOINT_WIDTH-1:0] vector_dot_product;
    input Vector4_t a, b;

    begin
        vector_dot_product = fixed_point_multiply(a.x, b.x) + fixed_point_multiply(a.y, b.y) + fixed_point_multiply(a.z, b.z) + fixed_point_multiply(a.w, b.w);
    end
endfunction
