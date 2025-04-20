`include "Vector4.vh"

module TriangleBackFaceCull(
    // Screenspace - triangle vertices
    input Vector4_t i_v1, i_v2, i_v3,

    // result - active if the triangle is back facing, and should be culled
    output reg o_result
);

reg `FixedPoint_t r_cross;
Vector4_t r_d1;
Vector4_t r_d2;

always @(*)
begin
    r_d1 = vector_sub_vector(i_v1, i_v2);
    r_d2 = vector_sub_vector(i_v3, i_v1);
    r_cross = vector_cross_product_2d(r_d1, r_d2); 
    
    o_result = (r_cross >= 0);
end


endmodule;