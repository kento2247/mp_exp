//***********************************************************************
// test.v
// Testbench for MIPS procesor
//
// 2013-07-04	Created (by matutani)
// 2016-06-06	Target is changed from Spartan-3AN to Zynq-7000 (by matutani)
//***********************************************************************
`timescale 1ns/1ps
module test;
parameter	STEP = 8;	/* System clock is 125MHz (8nsec) */
reg		clk;
reg	[31:0]	counter;
reg	[3:0]	sw;
reg	[3:0]	btn;		/* Reset when all buttons are pushed */
wire	[3:0]	led;
wire	[10:0]	lcd;
wire	[7:0]	ioa;
wire	[7:0]	iob;

/* Generate clock to sequence tests */
always #(STEP/2)
	clk <= ~clk;

/* FPGA board to be tested */
fpga_top fpga (clk, sw, btn, led, lcd, ioa, iob);

/* Initialize test */
initial begin
	clk	<= 0;
	counter <= 0;
	sw	<= 4'b0000;
	btn	<= 4'b0000;
	#(STEP/4);
	btn	<= 4'b1111;		/* Reset ON */
	#(STEP*6);
	btn	<= 4'b0000;		/* Reset OFF */

	#(STEP*50);
	sw	<= 4'b0101;
	btn	<= 4'b0110;
	#(STEP*50);
	sw	<= 4'b0000;
	btn	<= 4'b0000;

	/* Specify the simulation time. Default value is (STEP*1000) nsec.
	 * If it is not enough, longer value should be specified. */
	#(STEP*1000);
	$finish;
end

/* Clock counter */
always@(posedge clk)
	counter <= counter + 1;

/* Monitoring memory write operations */
always @ (posedge fpga.clk_62p5mhz)
	if (fpga.memwrite) begin
		$display("Clk %5d  Writing value %8d to address 0x%h", 
				counter, fpga.writedata, fpga.dataadr);
	end

/* Monitoring Switches */
always @ (sw)
	$display("Clk %5d  SWITCH value changed to\t%4b", counter, sw);

/* Monitoring Buttons */
always @ (btn)
	$display("Clk %5d  BUTTON value changed to\t%4b", counter, btn);

/* Monitoring LED */
always @ (led)
	$display("Clk %5d  LED    value changed to\t%4b", counter, led);

/* Monitoring LCD */
always @ (lcd)
	$display("Clk %5d  LCD    value changed to\t%3b_%4b_%4b", counter,
			lcd[10:8], lcd[7:4], lcd[3:0]);

initial begin
	$dumpfile("dump.vcd");
	$dumpvars(0, test.fpga);
end
endmodule
