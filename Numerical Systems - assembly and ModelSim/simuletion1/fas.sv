// Full Adder/Subtractor template
module fas (
    input logic a,           // Input bit a
    input logic b,           // Input bit b
    input logic cin,         // Carry in
    input logic a_ns,        // A_nS (add/not subtract) control
    output logic s,          // Output S
    output logic cout        // Carry out
);

// Put your code here
// ------------------
logic w1,w2,w3,w4,w5,w6,w7,w8,w9,w10;

XOR2 #(.Tpdlh(6), .Tpdhl(6)) xor1 (.Z(w1), .A(a), .B(b));
XOR2 #(.Tpdlh(6), .Tpdhl(6)) xor2 (.Z(s), .A(w1), .B(cin));

NOT #(.Tpdlh(10),.Tpdhl(10)) not1 (.A(cin),.Z(w2));
NOT #(.Tpdlh(10),.Tpdhl(10)) not2 (.A(b),.Z(w3));
OR2 #(.Tpdlh(8),.Tpdhl(8)) or1 (.A(w2),.B(w3),.Z(w4));
NOT #(.Tpdlh(10),.Tpdhl(10)) not3 (.A(w4),.Z(w5));

XOR2 #(.Tpdlh(6), .Tpdhl(6)) xor3 (.Z(w6), .A(a), .B(a_ns));
XOR2 #(.Tpdlh(6), .Tpdhl(6)) xor4 (.Z(w7), .A(b), .B(cin));
NOT #(.Tpdlh(10),.Tpdhl(10)) not4 (.A(w7),.Z(w8));
OR2 #(.Tpdlh(8),.Tpdhl(8)) or2 (.A(w6),.B(w8),.Z(w9));
NOT #(.Tpdlh(10),.Tpdhl(10)) not5 (.A(w9),.Z(w10));


OR2 #(.Tpdlh(8),.Tpdhl(8)) or3 (.A(w5),.B(w10),.Z(cout));

// End of your code

endmodule
