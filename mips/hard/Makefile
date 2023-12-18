all:
	make clean
	vivado -mode batch -source script/bitgen.tcl
	#vivado -mode batch -source script/bitgen_z7.tcl
program:
	vivado -mode batch -source script/program.tcl
clean:
	-rm -f *.jou *.log *.bit *.html *.xml
	-rm -rf *.srcs *.cache *.hw *.ip_user_files *.xpr .Xil
