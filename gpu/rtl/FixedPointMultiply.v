module FixedPointMultiply #(
    parameter WIDTH = 32,               // full width of each value being added, and the result
    parameter FRACTION_WIDTH = 8
)
(
    input signed [WIDTH-1:0] i_a,
    input signed [WIDTH-1:0] i_b,
    output reg signed [WIDTH-1:0] o_result
);

reg [(WIDTH*2)-1:0] r_result;

always @(*)
begin    
    r_result = (i_a * i_b);
end

assign o_result = r_result[(WIDTH-1) + FRACTION_WIDTH:FRACTION_WIDTH];

endmodule
