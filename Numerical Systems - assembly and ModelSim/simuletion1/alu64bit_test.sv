// 64-bit ALU test bench template
module alu64bit_test;

logic [63:0] a;
logic [63:0] b;
logic cin;
logic [1:0] op;
logic [63:0] s;
logic cout;

alu64bit alu(.a(a),.b(b),.cin(cin),.op(op),.s(s),.cout(cout));

initial   begin 

	a ={64{1'b1}} ;
	b = 64'h0;
	op = 2'b10;
        cin = 1'b0;

#2340
	a   ={64{1'b1}} ;
	b   = 64'h1;
	op  = 2'b10;
        cin = 1'b0;


#2340
	a   ={64{1'b1}} ;
	b   = 64'h0;
	op  = 2'b10;
        cin = 1'b0;

// End of your code
  end


endmodule
