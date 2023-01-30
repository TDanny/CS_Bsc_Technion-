// 32X32 Multiplier test template
module mult32x32_test;

    logic clk;            // Clock
    logic reset;          // Reset
    logic start;          // Start signal
    logic [31:0] a;       // Input a
    logic [31:0] b;       // Input b
    logic busy;           // Multiplier busy indication
    logic [63:0] product; // Miltiplication product

// Put your code here
// ------------------
	 
	mult32x32 mult(.clk(clk) ,
					.reset(reset),
					.start(start),
					.a(a), .b(b) ,
					.busy(busy) , 
					.product(product)
					);
	
	initial begin
		clk = 1'b1;
		reset = 1'b1;
		start = 1'b0;
		a = 32'd0;
		b = 32'd0;
		
		#8
		reset = 1'b0;
		
		#2
		a= 32'd205811680;
		b= 32'd206018301;
		start = 1'b1 ; 
		
		#2
		
		start = 1'b0;
		
		
		
	end

	always begin
		#1
		clk= ~clk;
	end
// End of your code

endmodule

