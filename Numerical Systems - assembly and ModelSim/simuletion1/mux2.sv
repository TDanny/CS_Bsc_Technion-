// 2->1 multiplexer template
module mux2 (
    input logic d0,          // Data input 0
    input logic d1,          // Data input 1
    input logic sel,         // Select input
    output logic z           // Output
);

// Put your code here
// ------------------
logic d0not;
logic or1result;
logic topandresult;
logic d1not;
logic selnot;
logic or2Result;
logic botAndResult;
NOT #(.Tpdlh(10),.Tpdhl(5)) not1 (.A(d0),.Z(d0not));
OR2 #(.Tpdlh(8),.Tpdhl(1)) or1 (.A(d0not),.B(sel),.Z(or1Result));
NOT #(.Tpdlh(10),.Tpdhl(5)) not2 (.A(or1Result),.Z(topAndResult));
NOT #(.Tpdlh(10),.Tpdhl(5)) not3 (.A(sel),.Z(selnot));
NOT #(.Tpdlh(10),.Tpdhl(5)) not4 (.A(d1),.Z(d1not));
OR2 #(.Tpdlh(8),.Tpdhl(1)) or2 (.A(selnot),.B(d1not),.Z(or2Result));
NOT #(.Tpdlh(10),.Tpdhl(5)) not5 (.A(or2Result),.Z(botAndResult));
OR2 #(.Tpdlh(8),.Tpdhl(1)) or3 (.A(topAndResult),.B(botAndResult),.Z(z));

// End of your code

endmodule
