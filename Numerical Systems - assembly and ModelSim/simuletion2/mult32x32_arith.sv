// 32X32 Multiplier arithmetic unit template
module mult32x32_arith (
    input logic clk,             // Clock
    input logic reset,           // Reset
    input logic [31:0] a,        // Input a
    input logic [31:0] b,        // Input b
    input logic a_sel,           // Select one 2-byte word from A
    input logic b_sel,           // Select one 2-byte word from B
    input logic [1:0] shift_sel, // Select output from shifters
    input logic upd_prod,        // Update the product register
    input logic clr_prod,        // Clear the product register
    output logic [63:0] product  // Miltiplication product
);

// Put your code here
// ------------------
	logic [15:0] A_out;
	logic [15:0] B_out;
	logic [31:0] mult_result;
	logic [63:0] shift0;
	logic [63:0] shift16;
	logic [63:0] shift32;
	logic [63:0] mux4 ;
	//A mux
	always_comb begin
		case(a_sel)
			1'b0: A_out= a[15:0];
			1'b1: A_out= a[31:16];
		endcase
	end
	
	//A mux
	always_comb begin
		case(b_sel)
			1'b0: B_out= b[15:0];
			1'b1: B_out= b[31:16];
		endcase
	end
	
	
	always_comb begin
		mult_result = A_out*B_out;
		shift0 = mult_result;
		shift16 = mult_result << 16;
		shift32 = mult_result << 32;
		
		case(shift_sel)
			2'b00: mux4 = shift0;
			2'b01: mux4 = shift16;
			2'b11: mux4 = shift16;
			2'b10: mux4 = shift32;
		endcase
	end
	
	
	always_ff @(posedge clk, posedge reset) begin
		if(reset==1'b1) begin
			product<=64'h0000;
		end
		else begin
			if(clr_prod==1'b1) begin
			product<=64'h0000;
			end
			else if(upd_prod==1'b1) begin
				product = product + mux4;
			end
		end
	end
// End of your code

endmodule
