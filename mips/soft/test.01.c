/* Do not remove the following line. Do not remove interrupt_handler(). */
#include "crt0.c"
void interrupt_handler(){}

void lcd_wait(int n)
{
	int i;
	for (i = 0; i < n; i++)
		;
}

void main()
{
	volatile int *sw_ptr = (int *)0xff04;
	volatile int *led_ptr = (int *)0xff08;
	// for (;;)
	//	*led_ptr = *sw_ptr;	

	volatile int *iob_ptr = (int *)0xff10;        *iob_ptr = 1; lcd_wait(7000000);
        *iob_ptr = 2; lcd_wait(7000000);        *iob_ptr = 3; lcd_wait(7000000);        *iob_ptr = 4; lcd_wait(7000000);        *iob_ptr = 5; lcd_wait(7000000);        *iob_ptr = 6; lcd_wait(7000000);        *iob_ptr = 7; lcd_wait(7000000);        *iob_ptr = 8; lcd_wait(7000000);        *iob_ptr = 9; lcd_wait(7000000);        *iob_ptr = 10; lcd_wait(7000000);        *iob_ptr = 11; lcd_wait(7000000);        *iob_ptr = 12; lcd_wait(7000000);        *iob_ptr = 13; lcd_wait(7000000);
}
