`include "FixedPoint.vh"
`include "Vector4.vh"

module TriangleRasterizerTiming (
    input i_clk,
    
    input i_reset_n,

    // ScreenSpace locations for vertices
    input Vector4_t i_v1, i_v2, i_v3,

    // signal when the module should start timing rasterization
    input i_start,

    // FixedPoint: x,y screen location of rasterized pixel
    output signed [31:0] o_x, o_y,

    // signals when the module is ready to start rasterizing a new triangle
    output reg o_ready,

    // signals when o_x, o_y are valid
    output reg o_valid
);

reg `FixedPoint_t r_x_min;
reg `FixedPoint_t r_x_max;
reg `FixedPoint_t r_y_min;
reg `FixedPoint_t r_y_max;
reg `FixedPoint_t r_x;
reg `FixedPoint_t r_y;

typedef enum logic {
    READY,
    RASTERIZE
} state_t;

state_t state;

always @(posedge i_clk or negedge i_reset_n)
begin
    if (!i_reset_n)
    begin
        state <= READY;
        r_x <= 0;
        r_y <= 0;
    end
    else begin
        if (i_start)
        begin
            r_x_min <= fixed_point_min3(i_v1.x, i_v2.x, i_v3.x);
            r_x_max <= fixed_point_max3(i_v1.x, i_v2.x, i_v3.x);
            r_y_min <= fixed_point_min3(i_v1.y, i_v2.y, i_v3.y);
            r_y_max <= fixed_point_max3(i_v1.y, i_v2.y, i_v3.y);
            
            state <= RASTERIZE;
            
            r_x <= fixed_point_min3(i_v1.x, i_v2.x, i_v3.x);
            r_y <= fixed_point_min3(i_v1.y, i_v2.y, i_v3.y);
        end
        else if (state == RASTERIZE)
        begin
            if (r_x < r_x_max)
            begin
                r_x <= r_x + uint8_to_fixed_point(1);
            end
            else
            begin
                if (r_y < r_y_max)
                begin
                    r_x <= r_x_min;                
                    r_y <= r_y + uint8_to_fixed_point(1);
                end else begin
                    r_x <= 0;
                    r_y <= 0;
                    state <= READY;
                end

            end
        end
    end
end

always @(*)
begin
    o_ready = 1;
    o_valid = 0;

    case (state)
    READY: begin
        o_ready = 1;
        o_valid = 0;
        o_x = 0;
        o_y = 0;
    end
    RASTERIZE: begin
        o_ready = 0;
        o_valid = 1;
        o_x = fixed_point_to_int32(r_x);
        o_y = fixed_point_to_int32(r_y);
    end
    endcase
end

endmodule
