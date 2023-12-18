//***********************************************************************
// top.v
// Top level system including MIPS, memory, and I/Os
//
// 2013-07-04   Created (by matutani)
// 2013-10-07   Byte enable is added (by matutani)
// 2016-06-03   Target is changed from Spartan-3AN to Zynq-7000 (by matutani)
// 2019-08-30   100msec timer is added (by matutani)
//***********************************************************************
`timescale 1ns/1ps
module fpga_top (
	input			clk_125mhz,
	input		[3:0]	sw,
	input		[3:0]	btn,
	output 	reg	[3:0]	led,
	output 	reg	[10:0]	lcd,
	output	reg	[7:0]	ioa,
	output	reg	[7:0]	iob,
	output	reg	[7:0]	ioc

	
);

wire	[31:0]	pc, instr, readdata, readdata0, readdata1, writedata, dataadr;
wire	[3:0]	byteen;
wire		reset;
wire		memwrite, memtoregM, swc, cs0, cs1, cs2, cs3, cs4, cs5, irq;
reg		clk_62p5mhz;

reg [7:0] mode;
wire buzz;
beep beep (clk_62p5mhz, reset, mode, buzz);

/* Reset when two buttons are pushed */
assign	reset	= btn[0] & btn[1];

/* 62.5MHz clock */
always @ (posedge clk_125mhz)
	if (reset)	clk_62p5mhz	<= 1;
	else		clk_62p5mhz	<= ~clk_62p5mhz;

/* CPU module (@62.5MHz) */
mips mips (clk_62p5mhz, reset, pc, instr, {7'b0000000, irq}, memwrite, 
	memtoregM, swc, byteen, dataadr, writedata, readdata, 1'b1, 1'b1);

/* Memory(cs0), Switch(cs1), LED(cs2), LCD(cs3), and more ... */
assign	cs0	= dataadr <  32'hff00;
assign	cs1	= dataadr == 32'hff04;
assign	cs2	= dataadr == 32'hff08;
assign	cs3	= dataadr == 32'hff0c;
assign  cs4 = dataadr == 32'hff10;

assign	readdata	= cs0 ? readdata0 : cs1 ? readdata1 : 0;

/* Memory module (@125MHz) */
mem mem (clk_125mhz, reset, cs0 & memwrite, pc[15:2], dataadr[15:2], instr, 
		readdata0, writedata, byteen);

/* Timer module (@62.5MHz) */
timer timer (clk_62p5mhz, reset, irq);

/* cs1 */
assign	readdata1	= {24'h0, btn, sw};
/* cs2 */
always @ (posedge clk_62p5mhz or posedge reset)
	if (reset)			led	<= 0;
	else if (cs2 && memwrite)	led	<= writedata[3:0];
/* cs3 */
always @ (posedge clk_62p5mhz or posedge reset)
	if (reset)			lcd	<= 0;
	else if (cs3 && memwrite)	lcd	<= writedata[10:0];

always @ (posedge clk_62p5mhz or posedge reset)       
	if (reset)                      mode    <= 0;       
	else if (cs4 && memwrite)       mode    <= writedata[7:0];
	
always @ (posedge clk_62p5mhz or posedge reset)        
	if (reset)                      iob     <= 0;        
	else                            iob[0]  <= buzz;

endmodule

//***********************************************************************
// 100msec timer for 62.5MHz clock
//
// 2019-08-30 Created (by matutani)
//***********************************************************************
module timer (
	input			clk, reset,
	output			irq
);
reg	[22:0]	counter;

assign	irq = (counter == 23'd6250000);

always @ (posedge clk or posedge reset)
	if (reset) 			counter	<= 0;
	else if (counter < 23'd6250000)	counter	<= counter + 1;
	else 				counter	<= 0;
endmodule

//***********************************************************************
// Memory (32bit x 16384word) with synchronous read ports for BlockRAM
//
// 2013-07-04   Created (by matutani)
// 2013-10-07   Byte enable is added (by matutani)
// 2016-06-03   Memory size is changed from 8192 to 16384 words (by matutani)
//***********************************************************************
module mem (
	input			clk, reset, memwrite,
	input		[13:0]	instradr, dataadr,
	output	reg	[31:0]	instr,
	output	reg	[31:0]	readdata,
	input		[31:0]	writedata,
	input		[3:0]	byteen
);
reg	[31:0]	RAM [0:16383];	/* Memory size is 16384 words */
wire	[7:0]	byte0, byte1, byte2, byte3;

assign	byte0	= byteen[0] ? writedata[ 7: 0] : readdata[ 7: 0];
assign	byte1	= byteen[1] ? writedata[15: 8] : readdata[15: 8];
assign	byte2	= byteen[2] ? writedata[23:16] : readdata[23:16];
assign	byte3	= byteen[3] ? writedata[31:24] : readdata[31:24];

always @ (posedge clk) begin
	if (memwrite)
		RAM[dataadr]	<= {byte3, byte2, byte1, byte0};
	instr	<= RAM[instradr];
	readdata<= RAM[dataadr];
end

/* Specify your program image file (e.g., program.dat) */
initial $readmemh("program.dat", RAM, 0, 16383);
endmodule

module beep
(
	input clk_62p5mhz,
	input reset,
	input [7:0] mode,
	output buzz
);

reg  [31:0] count;
wire [31:0] interval;

assign interval =  (mode ==  1) ? 14931 * 2: /* C  */ 
                   (mode ==  2) ? 14093 * 2: /* C# */
		  		   (mode ==  3) ? 13302 * 2: /* D  */                      
				   (mode ==  4) ? 12555 * 2: /* D# */      
				   (mode ==  5) ? 11850 * 2: /* E  */   
				   (mode ==  6) ? 11185 * 2: /* F  */    
				   (mode ==  7) ? 10558 * 2: /* F# */   
				   (mode ==  8) ?  9965 * 2: /* G  */    
				   (mode ==  9) ?  9406 * 2: /* G# */     
				   (mode == 10) ?  8878 * 2: /* A  */    
				   (mode == 11) ?  8380 * 2: /* A# */   
				   (mode == 12) ?  7909 * 2: /* B  */    
				   (mode == 13) ?  7465 * 2: /* C  */
				   0;

assign buzz = (mode > 0) && (count < interval / 2) ? 1 : 0;
always @ (posedge clk_62p5mhz or posedge reset)
	if (reset)
		count <= 0;
	else if (mode > 0)
		if (count < interval)
			count <= count + 1;
		else 
			count <= 0;

endmodule