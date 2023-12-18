/* Do not remove the following line. Do not remove interrupt_handler(). */
#include "crt0.c"
void interrupt_handler(){}

void main()
{
	volatile int *sw_ptr = (int *)0xff04;
	volatile int *led_ptr = (int *)0xff08;
	for (;;)
		*led_ptr = *sw_ptr;
}
