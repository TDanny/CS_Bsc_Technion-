// 1-bit ALU template
module alu1bit (
    input logic a,           // Input bit a
    input logic b,           // Input bit b
    input logic cin,         // Carry in
    input logic [1:0] op,    // Operation
    output logic s,          // Output S
    output logic cout        // Carry out
);

// Put your code here
// ------------------
logic w1,w2,w3,w4,w5;
OR2 #(.Tpdlh(8),.Tpdhl(8)) g1 (.A(a),.B(b),.Z(w1));
NOT #(.Tpdlh(10),.Tpdhl(10)) g5 (.A(w1),.Z(w2));
XOR2#(.Tpdlh(6),.Tpdhl(6)) g2 (.A(a),.B(b),.Z(w3));
NOT #(.Tpdlh(10),.Tpdhl(10)) g6 (.A(op[0]),.Z(w5));
fas g3(.a(a),.b(b),.cin(cin),.a_ns(w5),.s(w4),.cout(cout));
mux4 m4(.d0(w2),.d1(w3),.d2(w4),.d3(w4),.sel(op),.z(s));


// End of your code

endmodule
