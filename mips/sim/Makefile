RTL	= ../hard/top.v ../hard/mips.v ../hard/parts.v ../hard/multdiv.v
all:
	iverilog test.v $(RTL)
	vvp a.out 
wave:
	gtkwave dump.vcd &
clean:
	-rm -f a.out dump.vcd
