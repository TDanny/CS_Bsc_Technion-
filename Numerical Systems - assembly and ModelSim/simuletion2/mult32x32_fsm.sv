// 32X32 Multiplier FSM
module mult32x32_fsm (
    input logic clk,              // Clock
    input logic reset,            // Reset
    input logic start,            // Start signal
    output logic busy,            // Multiplier busy indication
    output logic a_sel,           // Select one 2-byte word from A
    output logic b_sel,           // Select one 2-byte word from B
    output logic [1:0] shift_sel, // Select output from shifters
    output logic upd_prod,        // Update the product register
    output logic clr_prod         // Clear the product register
	);

// Put your code here
// ------------------
	typedef enum { idle_st , new_multiply , a0b0 , a1b0 , a0b1 } action;
	
	action current_state;
	action next_state;
	
	always_ff @(posedge clk, posedge reset) begin
		if(reset==1'b1) begin
			current_state <= idle_st;
		end
		else begin
			current_state <= next_state;
		end
	end
	
	always_comb begin
		next_state = current_state;
		busy = 1'b0;
		a_sel = 1'b0;
		b_sel = 1'b0;
		shift_sel = 2'b00;
		upd_prod = 1'b0;
		
		
		case (current_state)
			idle_st: begin
				if(start == 1'b1) begin
					next_state = new_multiply;
					clr_prod = 1'b1;
				end
			end
			
			new_multiply: begin
				next_state = a0b0;
				busy = 1'b1;
				a_sel = 1'b0;
				b_sel = 1'b0;
				shift_sel = 2'b00;
				upd_prod = 1'b1;
				clr_prod = 1'b0;
			
			end
			
			a0b0: begin
				next_state = a1b0;
				busy = 1'b1;
				a_sel = 1'b1;
				b_sel = 1'b0;
				shift_sel = 2'b01;
				upd_prod = 1'b1;
				clr_prod = 1'b0;
			
			end
			
			a1b0: begin
				next_state = a0b1;
				busy = 1'b1;
				a_sel = 1'b0;
				b_sel = 1'b1;
				shift_sel = 2'b01; 
				upd_prod = 1'b1;
				clr_prod = 1'b0;
			
			end
			
			a0b1: begin
				next_state = idle_st;
				busy = 1'b1;
				a_sel = 1'b1;
				b_sel = 1'b1;
				shift_sel = 2'b10; 
				upd_prod = 1'b1;
				clr_prod = 1'b0;
			
			end
		endcase
	end
// End of your code

endmodule
