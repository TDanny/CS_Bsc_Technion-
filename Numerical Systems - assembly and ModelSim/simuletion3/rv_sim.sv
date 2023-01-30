// **********************************************************************
// Technion EE 044252: Digital Systems and Computer Structure course    *
// Simple Multicycle RISC-V model                                       *
// ==============================                                       *
// Top level                                                            *
// **********************************************************************
module rv_sim;

    // Simulation parameters
    localparam DPWIDTH = 32;
    localparam PERIOD = 10;
    localparam IMEM_FILE = "imem.hex";
    localparam DMEM_INFILE = "dmem_init.hex";
    localparam DMEM_OUTFILE = "dmem_out.hex";
    localparam IMEM_SIZE = 1024;
    localparam LOGIMEM_SIZE = 10;
    localparam DMEM_SIZE = 1024;
    localparam LOGDMEM_SIZE = 10;
    localparam TIMEOUT = 10000;

    // Design parameters
    `include "params.inc"

    logic clk;
    logic rst;

    logic [31:0] imem [0:IMEM_SIZE-1];
    logic [31:0] dmem [0:DMEM_SIZE-1];

    logic [DPWIDTH-1:0] imem_addr;
    logic [DPWIDTH-1:0] dmem_addr;
    logic [DPWIDTH-1:0] dmem_dataout;
    logic memrw;

    // Support only 32-bit aligned access
    logic [LOGDMEM_SIZE-1:0] dmem_addr_short;
    logic [LOGDMEM_SIZE-1:0] imem_addr_short;
    logic [DPWIDTH-1:0] dmem_datain;
    logic [DPWIDTH-1:0] imem_datain;

    assign dmem_addr_short = dmem_addr[LOGDMEM_SIZE+1:2];
    assign imem_addr_short = imem_addr[LOGIMEM_SIZE+1:2];
    assign dmem_datain = dmem[dmem_addr_short];
    assign imem_datain = imem[imem_addr_short];

    // The data memory
    always_ff @(posedge clk) begin
        if (memrw)  begin
            dmem[dmem_addr_short] <= dmem_dataout;
        end
    end

    // The RISC-V
    // ==========
    rv_top dut (
        // Memory interface
        .imem_addr(imem_addr),
        .dmem_addr(dmem_addr),
        .dmem_dataout(dmem_dataout),
        .memrw(memrw),
        .dmem_datain(dmem_datain),
        .imem_datain(imem_datain),
        // Clock and reset
        .clk(clk),
        .rst(rst)
    );

    // Simulation
    // ==========
    always begin
        #PERIOD clk <= ~clk;
    end

    // Initializations
    initial begin
        clk = 1'b0;
        rst = 1'b1;
        $readmemh(IMEM_FILE, imem);
        $readmemh(DMEM_INFILE, dmem);

        repeat(5) @(posedge clk);
        #1 rst = 1'b0;

        // Now the processor should start running
        #TIMEOUT;
        $error("No completion sequence detected. Probably something went wrong. Exiting on timeout.");
        $stop;
    end

    // Wait for the completion write sequence
    always_ff @(posedge clk) begin
        if (memrw && (dmem_dataout == 32'hDEAD) && (dmem_addr == 32'hFFFF)) begin
            $writememh(DMEM_OUTFILE, dmem);
            $info("Detected completion write sequence. Exiting.");
            $stop;
        end
    end

    // Monitor
    // =======
    always_ff @(posedge clk) begin
        if (!rst && (dut.ctl.current == 1)) begin
            // State machine is in decode state.
            $info("*** New command start: PC = %X, IR = %X", dut.dp.pcc, dut.dp.ir);
        end
    end

endmodule
