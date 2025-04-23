`include "Vector4.vh"

module Lighting(
    input Vector4_t i_colour,               // Vertex Colour
    input Vector4_t i_normal,               // Vertex Normal (World Space)
    input Vector4_t i_light_direction,      // direction of directional light
        
    output Vector4_t o_colour               // Lit Vertex Colour
);

reg `FixedPoint_t r_diffuse;
Vector4_t r_vertex_to_light;

reg `FixedPoint_t r_coeff_diffuse;
reg `FixedPoint_t r_coeff_ambient;

reg `FixedPoint_t r_light;

always @(*)
begin
    r_coeff_diffuse = uint8_to_fixed_point(3) >> 2;     // 0.75
    r_coeff_ambient = uint8_to_fixed_point(1) >> 2;     // 0.25

    r_vertex_to_light = vector_sub_vector(0, i_light_direction);

    r_diffuse = fixed_point_max(
                    0, 
                    vector_dot_product(
                        i_normal, 
                        r_vertex_to_light
                    )
                );

    r_light = fixed_point_add(
                    r_coeff_ambient,
                    fixed_point_multiply(
                        r_coeff_diffuse,
                        r_diffuse
                    )
                );

    o_colour = vector_multiply_scalar(
                    i_colour,
                    r_light
                );
end

endmodule
