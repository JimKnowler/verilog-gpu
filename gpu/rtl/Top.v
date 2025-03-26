module Top (
    input [7:0] a,
    input [7:0] b,
    output reg [7:0] sum
);



always @(*)
begin
    sum = a + b;
end

endmodule 
