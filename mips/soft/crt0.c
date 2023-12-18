extern unsigned int __sdata, __edata, __sbss, __ebss, __sbackup, __ebackup;
asm(
".global __start__		\n\t"
"__start__:			\n\t"
/* Don't use "li $sp, 0xff00" which is translated into "ori $sp, 0xff00" 
   that does not initialize the upper-half of stack pointer. */
"	lui $sp, 0		\n\t"
"	ori $sp, 0xff00		\n\t"
"	li $gp, 0		\n\t"
/* c0_sr[RE] = 1, c0_sr[IM0] = 1, and c0_sr[IE] = 1 */
"	li $k0, 0x02000101	\n\t"
"	mtc0 $k0, $12		\n\t"
);
void __reset__()
{
	unsigned int *src, *dst;
	/* Copy from .backup to .data */
	src = &__sbackup; dst = &__sdata;
	while (dst < &__edata)
		*(dst++) = *(src++);
	/* Clear .bss */
	for (dst = &__sbss; dst < &__ebss; dst++)
		*dst = 0;
	asm("j main");
}
asm(
/* Interrupt handler address is set to 0x0100. See fetchstage in mips.v.
   Be sure that __vector__ is located in 0x0100. See program.dump. */
"	nop			\n\t"
"	nop			\n\t"
"	nop			\n\t"
"	nop			\n\t"
"	nop			\n\t"
"__vector__:			\n\t"
".set noat			\n\t"
/* The original stack pointer is stored in register k0. */
"	move $k0, $sp		\n\t"
/* Stack pointer for interrupt handler is set to 0xc000. */
"	lui $sp, 0		\n\t"
"	ori $sp, 0xc000		\n\t"
/* All registers (except zero, k0, and k1) are stored in the new stack. */
"	addiu $sp, $sp, -128	\n\t"
"	sw $k0, 124($sp)	\n\t"
"	sw $at, 120($sp)	\n\t"
".set at			\n\t"
"	sw $v0, 116($sp)	\n\t"
"	sw $v1, 112($sp)	\n\t"
"	sw $a0, 108($sp)	\n\t"
"	sw $a1, 104($sp)	\n\t"
"	sw $a2, 100($sp)	\n\t"
"	sw $a3,  96($sp)	\n\t"
"	sw $t0,  92($sp)	\n\t"
"	sw $t1,  88($sp)	\n\t"
"	sw $t2,  84($sp)	\n\t"
"	sw $t3,  80($sp)	\n\t"
"	sw $t4,  76($sp)	\n\t"
"	sw $t5,  72($sp)	\n\t"
"	sw $t6,  68($sp)	\n\t"
"	sw $t7,  64($sp)	\n\t"
"	sw $s0,  60($sp)	\n\t"
"	sw $s1,  56($sp)	\n\t"
"	sw $s2,  52($sp)	\n\t"
"	sw $s3,  48($sp)	\n\t"
"	sw $s4,  44($sp)	\n\t"
"	sw $s5,  40($sp)	\n\t"
"	sw $s6,  36($sp)	\n\t"
"	sw $s7,  32($sp)	\n\t"
"	sw $t8,  28($sp)	\n\t"
"	sw $t9,  24($sp)	\n\t"
"	sw $gp,  20($sp)	\n\t"
"	sw $s8,  16($sp)	\n\t"
"	sw $ra,  12($sp)	\n\t"
/* Call interrupt handler. */
"	jal interrupt_handler	\n\t"
/* All registers (except zero, k0, and k1) are restored from the new stack. */
"	lw $ra,  12($sp)	\n\t"
"	lw $s8,  16($sp)	\n\t"
"	lw $gp,  20($sp)	\n\t"
"	lw $t9,  24($sp)	\n\t"
"	lw $t8,  28($sp)	\n\t"
"	lw $s7,  32($sp)	\n\t"
"	lw $s6,  36($sp)	\n\t"
"	lw $s5,  40($sp)	\n\t"
"	lw $s4,  44($sp)	\n\t"
"	lw $s3,  48($sp)	\n\t"
"	lw $s2,  52($sp)	\n\t"
"	lw $s1,  56($sp)	\n\t"
"	lw $s0,  60($sp)	\n\t"
"	lw $t7,  64($sp)	\n\t"
"	lw $t6,  68($sp)	\n\t"
"	lw $t5,  72($sp)	\n\t"
"	lw $t4,  76($sp)	\n\t"
"	lw $t3,  80($sp)	\n\t"
"	lw $t2,  84($sp)	\n\t"
"	lw $t1,  88($sp)	\n\t"
"	lw $t0,  92($sp)	\n\t"
"	lw $a3,  96($sp)	\n\t"
"	lw $a2, 100($sp)	\n\t"
"	lw $a1, 104($sp)	\n\t"
"	lw $a0, 108($sp)	\n\t"
"	lw $v1, 112($sp)	\n\t"
"	lw $v0, 116($sp)	\n\t"
".set noat			\n\t"
"	lw $at, 120($sp)	\n\t"
"	lw $k0, 124($sp)	\n\t"
/* The original stack pointer is restored in register sp. */
"	move $sp, $k0		\n\t"
"	mfc0 $k1, $14		\n\t"
"	nop			\n\t"
/* Be sure that "rfe" is not placed in a delay slot. See program.dump. */
"	rfe			\n\t"
"	nop			\n\t"
"	jr $k1			\n\t"
"	nop			\n\t"
);

/* gcc tries to use memcpy() for fast string copy but a bare metal app 
   may not have libc, so it is added here. */
void *memcpy(void *dest, const void *src, int n)
{
	char *dp = dest;
	const char *sp = src;
	while (n--)
		*dp++ = *sp++;
	return dest;
}
