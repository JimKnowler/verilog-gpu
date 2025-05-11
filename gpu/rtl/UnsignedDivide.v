module UnsignedDivide #(
    parameter WIDTH = 32
) (
    input i_reset_n,
    input i_clk,

    input i_start,                      // high for one tick, when starting a division
    input [WIDTH-1:0] i_dividend,       // must be valid when i_start is high
    input [WIDTH-1:0] i_divisor,        // must be valid when i_start is high

    output reg o_ready,                     // high when ready to start a new division
    output reg o_valid,                     // high for one tick when calculation is complete
    output reg [WIDTH-1:0] o_quotient,      // valid while o_valid is high
    output reg [WIDTH-1:0] o_remainder      // valid while o_valid is high
);

reg r_is_busy;
reg [$clog2(WIDTH): 0] r_step;

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
    begin
        r_step <= 0;
        r_is_busy <= 0;
    end 
    else 
    begin
        if (i_start)
        begin
            r_step <= 0;
            r_is_busy <= 1;

            // TODO: cache dividend and divisor
            
        end
        else if (r_is_busy)
        begin
            if (r_step < WIDTH)
            begin
                r_step <= r_step + 1;
            end
            
            if (r_step >= (WIDTH - 1))
            begin
                r_is_busy <= 0;
            end
        end
        else
        begin
            r_step <= 0;
        end
    end
end


always @(*)
begin
    o_ready = !r_is_busy;
    o_valid = (r_step == WIDTH);
    o_quotient = 0;
    o_remainder = 0;
end


endmodule
