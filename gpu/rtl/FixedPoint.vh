`ifndef FIXEDPOINT_VH
`define FIXEDPOINT_VH

`define FIXEDPOINT_WIDTH 32
`define FIXEDPOINT_FRACTION 17

`define FixedPoint_t signed [`FIXEDPOINT_WIDTH-1:0]

function `FixedPoint_t fixed_point_add;
    input `FixedPoint_t a, b;
    
    begin
        fixed_point_add = a + b;
    end
endfunction

function `FixedPoint_t fixed_point_sub;
    input `FixedPoint_t a, b;
    
    begin
        fixed_point_sub = a - b;
    end
endfunction

function `FixedPoint_t fixed_point_multiply;
    input `FixedPoint_t a, b;
    
    begin
        reg [(`FIXEDPOINT_WIDTH*2)-1:0] r_result;
        r_result = (a * b);
        
        fixed_point_multiply = r_result[(`FIXEDPOINT_WIDTH-1) + `FIXEDPOINT_FRACTION:`FIXEDPOINT_FRACTION];
    end
endfunction

function `FixedPoint_t fixed_point_divide;
    input `FixedPoint_t a, b;
    
    begin
        reg signed [(`FIXEDPOINT_WIDTH + `FIXEDPOINT_FRACTION)-1:0] r_a_shifted;
        reg signed [(`FIXEDPOINT_WIDTH + `FIXEDPOINT_FRACTION)-1:0] r_b_expanded;
        reg signed [(`FIXEDPOINT_WIDTH + `FIXEDPOINT_FRACTION)-1:0] r_result;

        r_a_shifted = a <<< `FIXEDPOINT_FRACTION;
        r_b_expanded = {{`FIXEDPOINT_FRACTION{b[`FIXEDPOINT_WIDTH-1]}}, b};
        r_result = r_a_shifted / r_b_expanded;
                
        fixed_point_divide = r_result[`FIXEDPOINT_WIDTH-1:0];
    end
endfunction

function [7:0] fixed_point_to_uint8;
    input `FixedPoint_t a;

    begin
        reg `FixedPoint_t a_shifted;
        a_shifted = a >>> `FIXEDPOINT_FRACTION;

        fixed_point_to_uint8 = a_shifted[7:0];
    end
endfunction

function `FixedPoint_t uint8_to_fixed_point;
    input signed [7:0] a;

    begin
        reg [`FIXEDPOINT_WIDTH - 8 - 1:0] r_padding = 0;
        uint8_to_fixed_point = {r_padding, a} << `FIXEDPOINT_FRACTION;
    end
endfunction

function `FixedPoint_t int32_to_fixed_point;
    input signed [31:0] a;

    begin
        int32_to_fixed_point = a <<< `FIXEDPOINT_FRACTION;
    end
endfunction

function signed [31:0] fixed_point_to_int32;
    input `FixedPoint_t a;

    begin
        fixed_point_to_int32 = a >>> `FIXEDPOINT_FRACTION;
    end
endfunction

function `FixedPoint_t fixed_point_min3;
    input `FixedPoint_t a, b, c;

    begin
        fixed_point_min3 = (a < b) 
                ? ((a < c) ? a : c) 
                : ((b < c) ? b : c);
    end
endfunction

function `FixedPoint_t fixed_point_max3;
    input `FixedPoint_t a, b, c;

    begin
        fixed_point_max3 = (a > b) 
                ? ((a > c) ? a : c) 
                : ((b > c) ? b : c);
    end
endfunction

function `FixedPoint_t fixed_point_min;
    input `FixedPoint_t a, b;

    begin
        fixed_point_min = (a < b) 
                ? a
                : b;
    end
endfunction

function `FixedPoint_t fixed_point_max;
    input `FixedPoint_t a, b;

    begin
        fixed_point_max = (a > b) 
                ? a 
                : b;
    end
endfunction

`endif // FIXEDPOINT_VH
