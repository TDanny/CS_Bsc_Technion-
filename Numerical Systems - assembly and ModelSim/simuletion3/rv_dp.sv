// **********************************************************************
// Technion EE 044252: Digital Systems and Computer Structure course    *
// Simple Multicycle RISC-V model                                       *
// ==============================                                       *
// Data path                                                            *
// **********************************************************************
 module rv_dp
    #(parameter
        DPWIDTH = 32,
        RFSIZE  = 32
    )
 (

     // Memory interface
     output logic [DPWIDTH-1:0] imem_addr,
     output logic [DPWIDTH-1:0] dmem_addr,
     output logic [DPWIDTH-1:0] dmem_dataout,
     input logic [DPWIDTH-1:0] dmem_datain,
     input logic [DPWIDTH-1:0] imem_datain,

     // Interface with control logic
     output logic [DPWIDTH-1:0] instr,
     output logic zero,
     input logic pcsourse,
     input logic pcwrite,
     input logic pccen,
     input logic irwrite,
     input logic [1:0] wbsel,
     input logic regwen,
     input logic [1:0] immsel,
     input logic [1:0] asel,
	 input logic bff,
     input logic bsel,
     input logic [3:0] alusel,
     input logic mdrwrite,
     
     // Clock and reset
     input logic clk,
     input logic rst
 );

 // Design parameters
 `include "params.inc"

 // Stage registers
 logic [DPWIDTH-1:0] pc, pcc, ir, a, b, aluout, mdr;


 // Fetch
 assign imem_addr = pc;

 // PC
 // ==
 always_ff @(posedge clk or posedge rst)
     if (rst)
         pc     <= 0;
     else if (pcwrite)
         pc     <= (pcsourse == PC_ALU) ? aluout : pc + 4;
 
 // PCC
 // ===
 always_ff @(posedge clk or posedge rst)
     if (rst)
         pcc    <= 0;
     else if (pccen)
         pcc    <= pc;
 
 // IR
 // ==
 always_ff @(posedge clk or posedge rst)
     if (rst)
         ir     <= 0;
     else if (irwrite)
         ir     <= imem_datain;
 assign instr = ir;
 
 // Register file inputs
 // ====================
 logic [DPWIDTH-1:0] datad;
 always_comb
    case (wbsel)
        WB_MDR:     datad = mdr;
        WB_ALUOUT:  datad = aluout;
        WB_PC:      datad = pc;
        default:    datad = pc;
    endcase
 logic [4:0] addra, addrb, addrd;
 assign addra = ir[19:15];
 assign addrb = ir[24:20];
 assign addrd = ir[11:7];

 
 // Register File
 // =============
 logic [DPWIDTH-1:0] rf [RFSIZE-1:1];

 always_ff @(posedge clk or posedge rst)
     if (regwen && (addrd != 0)) // X0 is constant 0
         rf[addrd]    <= datad;

 always_ff @(posedge clk or posedge rst)
     if (rst)
     begin
         a      <= 0;
         b      <= 0;
     end
     else begin
         a <= (addra == 0) ? 0 : rf[addra];
		 if(bff==REGULAR_B) begin
			b      <= (addrb == 0) ? 0 : rf[addrb];
		end 
		else begin
			b = aluout;
		end
     end

 // ALU
 // ===
 
 // Immediate selector
 logic [DPWIDTH-1:0] imm;
 always_comb
 begin
     case(immsel)
         IMM_J: imm = {{12{ir[31]}},ir[19:12],ir[20],ir[30:21],1'b0};
         IMM_B: imm = {{20{ir[31]}},ir[7],ir[30:25],ir[11:8],1'b0};
         IMM_S: imm = {{21{ir[31]}},ir[30:25],ir[11:7]};
         IMM_L: imm = {{21{ir[31]}},ir[30:20]};
     endcase
 end

 // ALU input A
 logic [DPWIDTH-1:0] alu_a;
 //assign alu_a = (asel == ALUA_REG) ? a : pcc;
 //assign alu_a = (asel == ALUA_CONST) ? 0 : alu_a;

always_comb begin
     case (asel)
		 ALUA_REG: alu_a = a;
		 ALUA_CONST: alu_a = 0;
		 default: alu_a = pcc;
	endcase
end
 // ALU input A
 logic [DPWIDTH-1:0] alu_b;
 assign alu_b = (bsel == ALUB_REG) ? b : imm;

 // For signed comparison, cast to integer. logic is by default unsigned
 integer alu_as;
 always_comb alu_as = alu_a;
 integer alu_bs;
 always_comb alu_bs = alu_b;

 // The ALU
 logic [DPWIDTH-1:0] alu_result;
 always_comb
     case (alusel)
         ALU_ADD: alu_result = alu_a + alu_b;
         ALU_SUB: alu_result = alu_a - alu_b;
         ALU_SLL: alu_result = alu_a << alu_b;
         ALU_SLT: alu_result = (alu_as < alu_bs) ? 1 : 0;
         ALU_SLTU:alu_result = (alu_a < alu_b) ? 1 : 0;
         ALU_XOR: alu_result = alu_a ^ alu_b;
         ALU_SRL: alu_result = alu_a >> alu_b;
         ALU_SRA: alu_result = alu_a >>> alu_b;
         ALU_OR : alu_result = alu_a | alu_b;
         ALU_AND: alu_result = alu_a & alu_b;
         default: alu_result = alu_a + alu_b;
     endcase

 assign zero = (alu_result == 0);
 

 always_ff @(posedge clk or posedge rst)
     if (rst)
         aluout     <= 0;
     else
         aluout     <= alu_result;


 // Memory
 // ======
 assign dmem_addr = aluout;
 assign dmem_dataout = b;

 always_ff @(posedge clk or posedge rst)
     if (rst)
         mdr    <= 0;
     else if (mdrwrite)
         mdr    <= dmem_datain;

endmodule


