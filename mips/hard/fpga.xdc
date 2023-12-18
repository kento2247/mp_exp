##Clock signal
set_property PACKAGE_PIN L16 [get_ports clk_125mhz]
set_property IOSTANDARD LVCMOS33 [get_ports clk_125mhz]
create_clock -add -name sys_clk_pin -period 8.00 -waveform {0 4} [get_ports clk_125mhz]

##Switches
set_property PACKAGE_PIN G15 [get_ports {sw[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sw[0]}]
set_property PACKAGE_PIN P15 [get_ports {sw[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sw[1]}]
set_property PACKAGE_PIN W13 [get_ports {sw[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sw[2]}]
set_property PACKAGE_PIN T16 [get_ports {sw[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {sw[3]}]

##Buttons
set_property PACKAGE_PIN R18 [get_ports {btn[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {btn[0]}]
set_property PACKAGE_PIN P16 [get_ports {btn[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {btn[1]}]
set_property PACKAGE_PIN V16 [get_ports {btn[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {btn[2]}]
set_property PACKAGE_PIN Y16 [get_ports {btn[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {btn[3]}]

##LEDs
set_property PACKAGE_PIN M14 [get_ports {led[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[0]}]
set_property PACKAGE_PIN M15 [get_ports {led[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[1]}]
set_property PACKAGE_PIN G14 [get_ports {led[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[2]}]
set_property PACKAGE_PIN D18 [get_ports {led[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[3]}]

##Pmod Header JA (XADC)
set_property PACKAGE_PIN N15 [get_ports {ioc[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioc[0]}]
set_property PACKAGE_PIN L14 [get_ports {ioc[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioc[1]}]
set_property PACKAGE_PIN K16 [get_ports {ioc[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioc[2]}]
set_property PACKAGE_PIN K14 [get_ports {ioc[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioc[3]}]
##set_property PACKAGE_PIN N16 [get_ports {ioc[4]}]
##set_property IOSTANDARD LVCMOS33 [get_ports {ioc[4]}]
##set_property PACKAGE_PIN L15 [get_ports {ioc[5]}]
##set_property IOSTANDARD LVCMOS33 [get_ports {ioc[5]}]
##set_property PACKAGE_PIN J16 [get_ports {ioc[6]}]
##set_property IOSTANDARD LVCMOS33 [get_ports {ioc[6]}]
##set_property PACKAGE_PIN J14 [get_ports {ioc[7]}]
##set_property IOSTANDARD LVCMOS33 [get_ports {ioc[7]}]

##Pmod Header JB
set_property PACKAGE_PIN T20 [get_ports {lcd[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[0]}]
set_property PACKAGE_PIN U20 [get_ports {lcd[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[1]}]
set_property PACKAGE_PIN V20 [get_ports {lcd[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[2]}]
set_property PACKAGE_PIN W20 [get_ports {lcd[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[3]}]
set_property PACKAGE_PIN Y18 [get_ports {lcd[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[4]}]
set_property PACKAGE_PIN Y19 [get_ports {lcd[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[5]}]
set_property PACKAGE_PIN W18 [get_ports {lcd[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[6]}]
set_property PACKAGE_PIN W19 [get_ports {lcd[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[7]}]

##Pmod Header JC
set_property PACKAGE_PIN Y14 [get_ports {lcd[8]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[8]}]
set_property PACKAGE_PIN W14 [get_ports {lcd[9]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[9]}]
set_property PACKAGE_PIN T12 [get_ports {lcd[10]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[10]}]

##Pmod Header JD
set_property PACKAGE_PIN T14 [get_ports {ioa[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioa[0]}]
set_property PACKAGE_PIN T15 [get_ports {ioa[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioa[1]}]
set_property PACKAGE_PIN P14 [get_ports {ioa[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioa[2]}]
set_property PACKAGE_PIN R14 [get_ports {ioa[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioa[3]}]
set_property PACKAGE_PIN U14 [get_ports {ioa[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioa[4]}]
set_property PACKAGE_PIN U15 [get_ports {ioa[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioa[5]}]
set_property PACKAGE_PIN V17 [get_ports {ioa[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioa[6]}]
set_property PACKAGE_PIN V18 [get_ports {ioa[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ioa[7]}]

##Pmod Header JE
set_property PACKAGE_PIN V12 [get_ports {iob[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob[0]}]
set_property PACKAGE_PIN W16 [get_ports {iob[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob[1]}]
set_property PACKAGE_PIN J15 [get_ports {iob[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob[2]}]
set_property PACKAGE_PIN H15 [get_ports {iob[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob[3]}]
set_property PACKAGE_PIN V13 [get_ports {iob[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob[4]}]
set_property PACKAGE_PIN U17 [get_ports {iob[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob[5]}]
set_property PACKAGE_PIN T17 [get_ports {iob[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob[6]}]
set_property PACKAGE_PIN Y17 [get_ports {iob[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob[7]}]
