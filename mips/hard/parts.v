//***********************************************************************
// parts.v
// Authors
// David Harris David_Harris at hmc dot edu 3 November 2005
// Carl Nygaard carlpny at gmail dot com 2007
// Thomas W. Barr tbarr at cs dot hmc dot edu 2007
// Matt Totino mtotino at hmc.edu 2007
// Nathaniel Pinckney npinckney at gmail dot com
// Nate Schlossberg sonofernda at gmail dot com 2007
//
// Pipelined MIPS processor spare parts
//
// 2013-07-04   clk is used instead of ph1 and ph2 (by matutani)
//***********************************************************************
`timescale 1ns/1ps

module adder(input [31:0] a, b,
             output [31:0] y);
assign y = a + b;
endmodule

/* Adder with carry in and out */
module adderc #(parameter WIDTH = 32)
             (input [WIDTH-1:0]  a, b,
	      input              cin,
              output [WIDTH-1:0] y,
              output             cout);
assign {cout, y} = a + b + cin;
endmodule

module eqcmp #(parameter WIDTH = 32)
             (input [WIDTH-1:0] a, b,
              output            eq);
assign eq = (a == b);
endmodule

module eqzerocmp #(parameter WIDTH = 32)
                 (input [WIDTH-1:0]  a,
                  output        eq);
assign eq = (a == 0);
endmodule

module neqzerocmp #(parameter WIDTH = 32)
                 (input [WIDTH-1:0]  a,
                  output        eq);
assign eq = (a != 0);
endmodule

module gtzerocmp #(parameter WIDTH = 32)
                 (input [WIDTH-1:0] a,
                  output       eq);
assign eq = ~a[WIDTH-1] & (a[WIDTH-2:0] != 0);
endmodule

module ltzerocmp #(parameter WIDTH = 32)
                 (input [WIDTH-1:0] a,
                  output       eq);
assign eq = a[WIDTH-1];
endmodule

/* When disabled, signext acts as a zero extender */
module signext #(parameter INPUT = 16, OUTPUT = 32)
               (input  [INPUT-1:0] a,
               input  enable,
               output [OUTPUT-1:0] y);
wire extension;
assign extension = (enable ? a[INPUT-1] : 0);
assign y = {{OUTPUT-INPUT{extension}}, a};
endmodule

module flopenrc #(parameter WIDTH = 32)
                 (input                  clk, reset,
                  input                  en, clear,
                  input      [WIDTH-1:0] d, 
                  output reg [WIDTH-1:0] q);
always @(posedge clk or posedge reset)
	if	(reset)	q <= 0;
	else if (clear)	q <= 0;
	else if (en)	q <= d;
endmodule

module flopenr #(parameter WIDTH = 32)
                (input                  clk, reset,
                 input                  en,
                 input      [WIDTH-1:0] d, 
                 output reg [WIDTH-1:0] q);
always @(posedge clk or posedge reset)
	if	(reset) q <= 0;
	else if (en)	q <= d;
endmodule

module flopen #(parameter WIDTH = 32)
               (input                  clk,
                input                  en,
                input      [WIDTH-1:0] d, 
                output reg [WIDTH-1:0] q);
always @(posedge clk)
	if (en)	q <= d;
endmodule

module flopr #(parameter WIDTH = 32)
              (input                  clk, reset,
               input      [WIDTH-1:0] d, 
               output reg [WIDTH-1:0] q);
always @(posedge clk or posedge reset)
	if (reset)	q <= 0;
	else		q <= d;
endmodule

module mux2 #(parameter WIDTH = 32)
             (input  [WIDTH-1:0] d0, d1, 
              input              s, 
              output [WIDTH-1:0] y);
assign y = s ? d1 : d0; 
endmodule

module mux3 #(parameter WIDTH = 32)
             (input  [WIDTH-1:0] d0, d1, d2,
              input  [1:0]       s, 
              output [WIDTH-1:0] y);
assign y = s[1] ? d2 : (s[0] ? d1 : d0); 
endmodule

module mux4 #(parameter WIDTH = 32)
             (input  [WIDTH-1:0] d0, d1, d2, d3,
              input  [1:0]       s, 
              output [WIDTH-1:0] y);
assign y = s[1] ? (s[0] ? d3 : d2)
		: (s[0] ? d1 : d0); 
endmodule

/* Basic one hot decoders, eg b10 -> b0100 */
module dec2 (input  [1:0] x,
             output [3:0] y);
assign y = (x[0] ? (x[1] ? 4'b1000 : 4'b0010)
			: (x[1] ? 4'b0100 : 4'b0001));
endmodule

module and2 #(parameter WIDTH = 32)
             (input  [WIDTH-1:0] a, b,
              output [WIDTH-1:0] y);
assign y = a & b;
endmodule

module xor2 #(parameter WIDTH = 32)
             (input  [WIDTH-1:0] a, b,
              output [WIDTH-1:0] y);
assign y = a ^ b;
endmodule

module inc #(parameter WIDTH = 32)
            (input  [WIDTH-1:0] a,
             output [WIDTH-1:0] y,
             output             cout);
assign {cout, y} = a + 1'b1;
endmodule

module zerodetect #(parameter WIDTH = 32)
             (input  [WIDTH-1:0] a,
              output             y);
assign y = ~|a;
endmodule

module prienc_8 (input       [7:0]  a,
                  output reg [2:0]  y);
always @ (*)
	casex(a)
		/* Rearrange to set priority */
		8'b1xxxxxxx : y <= 3'b000;
		8'b01xxxxxx : y <= 3'b001;
		8'b001xxxxx : y <= 3'b010;
		8'b0001xxxx : y <= 3'b011;
		8'b00001xxx : y <= 3'b100;
		8'b000001xx : y <= 3'b101;
		8'b0000001x : y <= 3'b110;
		8'b00000001 : y <= 3'b111;
		default    : y <= 3'bxxx;
	endcase
endmodule
