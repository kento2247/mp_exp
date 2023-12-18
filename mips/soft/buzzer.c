/* Do not remove the following line. Do not remove interrupt_handler(). */
#include "crt0.c"
#include <stdio.h>

void interrupt_handler() {}

void lcd_wait(int n)
{
	int i;
	for (i = 0; i < n; i++)
		;
}

void main()
{
	// volatile int* sw_ptr = (int*)0xff04;
	// volatile int* led_ptr = (int*)0xff08;

	int i = 0;
	volatile int* iob_ptr = (int*)0xff10;
	for (i = 1; i < 14; i++) {
		printf("%d\n", i);
		*iob_ptr = i;
		lcd_wait(3500000);
		*iob_ptr = 0;
		lcd_wait(3500000);
	}
}
