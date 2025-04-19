/**
 * Apply a Projetion Matrix to a Vertex, and transform it into screenspace
 */

`include "Matrix44.vh"

module VertexTransform (
    input Vector4_t i_vertex,
    input Matrix44_t i_matrix,
    input `FixedPoint_t i_screenWidth,
    input `FixedPoint_t i_screenHeight,
    output Vector4_t o_vertex
);

Vector4_t r_vertexClipSpace;
Vector4_t r_vertexNDC;
Vector4_t r_vertexScreenSpace;
reg `FixedPoint_t r_halfScreenWidth;
reg `FixedPoint_t r_halfScreenHeight;

always @(*)
begin
    // multiply by projection matrix, to transform into clip space
    r_vertexClipSpace = matrix_multiply_vector(i_matrix, i_vertex);

    // divide by W, to transform to Normalised Device Co-ordinates (NDC)
    r_vertexNDC = vector_divide_scalar(r_vertexClipSpace, r_vertexClipSpace.w);

    // transform x,y into screen space
    r_halfScreenWidth = i_screenWidth >> 1;
    r_vertexScreenSpace.x = fixed_point_multiply(r_vertexNDC.x, r_halfScreenWidth);
    r_vertexScreenSpace.x = fixed_point_add(r_vertexScreenSpace.x, r_halfScreenWidth);
    
    r_halfScreenHeight = i_screenHeight >> 1;
    r_vertexScreenSpace.y = fixed_point_multiply(r_vertexNDC.y, r_halfScreenHeight);
    r_vertexScreenSpace.y = fixed_point_add(r_vertexScreenSpace.y, r_halfScreenHeight);

    r_vertexScreenSpace.z = r_vertexNDC.z;
    r_vertexScreenSpace.w = r_vertexNDC.w;

    // output vertex in screen space
    o_vertex = r_vertexScreenSpace;
end

endmodule
