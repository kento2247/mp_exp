ENTRY(__start__)
SECTIONS
{
	.text 	: { *(.text); . = ALIGN(4); }
	.rodata : { *(.rodata); . = ALIGN(4); }
	.data 	: { __sdata = .; *(.data); . = ALIGN(4); __edata = .; }
	.bss 	: { __sbss = .; *(.*bss .*common); . = ALIGN(4); __ebss = .; }
	.backup : { __sbackup = .; *(.backup); . = ALIGN(4); __ebackup = .; }
	/DISCARD/ : { *(.comment .gnu.attributes .reginfo .MIPS.abiflags); }
}
