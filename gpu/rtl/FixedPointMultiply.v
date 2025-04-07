module FixedPointMultiply #(
    parameter WIDTH = 32,               // full width of each value being added, and the result
    parameter FRACTION_WIDTH = 8
)
(
    input [WIDTH-1:0] i_a,
    input [WIDTH-1:0] i_b,
    output reg [(WIDTH*2)-1:0] o_result
);

always @(*)
begin
    o_result = (i_a * i_b) >> FRACTION_WIDTH;
end

endmodule
