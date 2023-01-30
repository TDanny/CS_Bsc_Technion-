// 32X32 Iterative Multiplier template
module mult32x32 (
    input logic clk,            // Clock
    input logic reset,          // Reset
    input logic start,          // Start signal
    input logic [31:0] a,       // Input a
    input logic [31:0] b,       // Input b
    output logic busy,          // Multiplier busy indication
    output logic [63:0] product // Miltiplication product
);

// Put your code here
// ------------------
	logic a_sel , b_sel ;
	logic [1:0]shift_sel;
	logic upd_prod, clr_prod;
 

	mult32x32_fsm fsm(.clk(clk) , .reset(reset), .start(start) , .busy(busy) , .a_sel(a_sel), .b_sel(b_sel) , .shift_sel(shift_sel) , .upd_prod(upd_prod), .clr_prod(clr_prod)); 
	mult32x32_arith arith(.clk(clk) , .reset(reset),.a(a) , .b(b), .a_sel(a_sel), .b_sel(b_sel) , .shift_sel(shift_sel) , .upd_prod(upd_prod), .clr_prod(clr_prod), .product(product)); 
	
// End of your code

endmodule
