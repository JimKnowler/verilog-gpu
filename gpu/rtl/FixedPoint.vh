`ifndef FIXEDPOINT_VH
`define FIXEDPOINT_VH

`define FIXEDPOINT_WIDTH 32
`define FIXEDPOINT_FRACTION 8

function signed [`FIXEDPOINT_WIDTH-1:0] fixed_point_add;
    input signed [`FIXEDPOINT_WIDTH-1:0] a, b;
    
    begin
        fixed_point_add = a + b;
    end
endfunction

function signed [`FIXEDPOINT_WIDTH-1:0] fixed_point_sub;
    input signed [`FIXEDPOINT_WIDTH-1:0] a, b;
    
    begin
        fixed_point_sub = a - b;
    end
endfunction

function signed [`FIXEDPOINT_WIDTH-1:0] fixed_point_multiply;
    input signed [`FIXEDPOINT_WIDTH-1:0] a, b;
    
    begin
        reg [(`FIXEDPOINT_WIDTH*2)-1:0] r_result;
        r_result = (a * b);
        
        fixed_point_multiply = r_result[(`FIXEDPOINT_WIDTH-1) + `FIXEDPOINT_FRACTION:`FIXEDPOINT_FRACTION];
    end
endfunction

function signed [`FIXEDPOINT_WIDTH-1:0] fixed_point_divide;
    input signed [`FIXEDPOINT_WIDTH-1:0] a, b;
    
    begin
        reg signed [(`FIXEDPOINT_WIDTH + `FIXEDPOINT_FRACTION)-1:0] r_a_shifted;
        reg signed [(`FIXEDPOINT_WIDTH + `FIXEDPOINT_FRACTION)-1:0] r_result;
        
        r_a_shifted = (a <<< `FIXEDPOINT_FRACTION);
        r_result = r_a_shifted / {`FIXEDPOINT_FRACTION'b0, b};
                
        fixed_point_divide = r_result[`FIXEDPOINT_WIDTH-1:0];
    end
endfunction

`endif // FIXEDPOINT_VH
