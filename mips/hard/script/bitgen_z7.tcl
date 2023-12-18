set project	"test1"
set program	"../soft/program.dat"

create_project	-force ${project} -part "xc7z020clg400-1"
import_files	-flat "top.v mips.v multdiv.v parts.v fpga_z7.xdc" ${program}

synth_design	-top "fpga_top"

opt_design
place_design 
route_design 

write_bitstream	-verbose -force "fpga_top.bit"
