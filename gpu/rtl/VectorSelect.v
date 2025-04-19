`include "Vector4.vh"

module VectorSelect (
    input Vector4_t i_vector,
    input [1:0] i_index,
    output `FixedPoint_t o_result
);

always @(*)
begin
    o_result = 0;
    
    case (i_index)
        0: begin
            o_result = i_vector.x;
        end
        1: begin
            o_result = i_vector.y;
        end
        2: begin
            o_result = i_vector.z;
        end
        3: begin
            o_result = i_vector.w;
        end
    endcase
end

endmodule