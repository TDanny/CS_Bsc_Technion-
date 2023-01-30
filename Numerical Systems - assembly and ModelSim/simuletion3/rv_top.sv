// **********************************************************************
// Technion EE 044252: Digital Systems and Computer Structure course    *
// Simple Multicycle RISC-V model                                       *
// ==============================                                       *
// Top level                                                            *
// **********************************************************************

 module rv_top
    #(parameter
        DPWIDTH = 32,
        RFSIZE  = 32
    )
 (

     // Memory interface
     output logic [DPWIDTH-1:0] imem_addr,
     output logic [DPWIDTH-1:0] dmem_addr,
     output logic [DPWIDTH-1:0] dmem_dataout,
     output logic memrw,
     input logic [DPWIDTH-1:0] dmem_datain,
     input logic [DPWIDTH-1:0] imem_datain,

     // Clock and reset
     input logic clk,
     input logic rst
 );


 // Interconnect wire declarations
 // ==============================
 logic [DPWIDTH-1:0] instr;
 logic zero;
 logic pcsourse;
 logic pcwrite;
 logic pccen;
 logic irwrite;
 logic [1:0] wbsel;
 logic regwen;
 logic [1:0] immsel;
 logic [1:0]asel;
 logic bsel;
 logic [3:0] alusel;
 logic mdrwrite;
 logic bff;

     
 // Data path
 // =========
 rv_dp
    #(
        .DPWIDTH(DPWIDTH),
        .RFSIZE(RFSIZE)
    ) dp
 (

     // Memory interface
     .imem_addr(imem_addr),
     .dmem_addr(dmem_addr),
     .dmem_dataout(dmem_dataout),
     .dmem_datain(dmem_datain),
     .imem_datain(imem_datain),

     // Interface with control logic
     .instr(instr),
     .zero(zero),
     .pcsourse(pcsourse),
     .pcwrite(pcwrite),
     .pccen(pccen),
     .irwrite(irwrite),
     .wbsel(wbsel),
     .regwen(regwen),
     .immsel(immsel),
     .asel(asel),
     .bsel(bsel),
     .alusel(alusel),
     .mdrwrite(mdrwrite),
	 .bff(bff),
	 
     
     // Clock and reset
     .clk(clk),
     .rst(rst)
 );

 // Control
 // =======
 rv_ctl ctl
 (

     // Output to memory
     .memrw(memrw),

     // Interface with datapath
     .instr(instr),
     .zero(zero),
     .pcsourse(pcsourse),
     .pcwrite(pcwrite),
     .pccen(pccen),
     .irwrite(irwrite),
     .wbsel(wbsel),
     .regwen(regwen),
     .immsel(immsel),
     .asel(asel),
     .bsel(bsel),
     .alusel(alusel),
     .mdrwrite(mdrwrite),
	 .bff(bff),
     // Clock and reset
     .clk(clk),
     .rst(rst)
 );
 
 endmodule
