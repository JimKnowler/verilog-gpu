module FixedPointAdd #(
    parameter WIDTH = 32
)
(
    input [WIDTH-1:0] i_a,
    input [WIDTH-1:0] i_b,
    output reg [WIDTH-1:0] o_result
);

always @(*)
begin
    o_result = i_a + i_b;
end

endmodule
