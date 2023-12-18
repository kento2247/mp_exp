/* Do not remove the following line. Do not remove interrupt_handler(). */
#include "crt0.c"

/* interrupt_handler() is called every 100msec */
void interrupt_handler()
{
	static int cnt = 0;
	volatile int *led_ptr = (int *)0xff08;
	cnt++;
	if (cnt % 10 == 0)
		*led_ptr = cnt / 10;
}

void main()
{
	for (;;);
}
