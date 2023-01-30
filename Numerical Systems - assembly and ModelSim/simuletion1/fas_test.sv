// Full Adder/Subtractor test bench template
module fas_test;

// Put your code here
// ------------------
logic a;
logic b;
logic cin;
logic a_ns;
logic s;
logic cout;

fas fas1(.a(a),.b(b),.cin(cin),.a_ns(a_ns),.s(s),.cout(cout));


initial begin
a=1;
b=0;
cin=0;
a_ns=1;

#100
a=1;
b=0;
cin=1;
a_ns=1;

#100
a=1;
b=0;
cin=0;
a_ns=1;

end
// End of your code

endmodule
