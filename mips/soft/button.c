/*
 * Switch functions
 */

#define IO_A_ADDR 0xff18 // between display and speaker
// #define IO_B_ADDR 0xff10  // speaker
#define IO_C_ADDR 0xff14 // under the display
#define BTN_ADDR 0xff04

int btn_check_num(int button_num)
{
  volatile int *btn_ptr = (int *)BTN_ADDR;
  switch (button_num) {
  case 0:
    return (*btn_ptr & 0x10) ? 1 : 0;
    break;
  case 1:
    return (*btn_ptr & 0x20) ? 1 : 0;
    break;
  case 2:
    return (*btn_ptr & 0x40) ? 1 : 0;
    break;
  case 3:
    return (*btn_ptr & 0x80) ? 1 : 0;
    break;
  default:
    return 0;
  }
}

int btn_check_a()
{
  volatile int *sw_ptr = (int *)IO_A_ADDR;
  return (*sw_ptr & 0x1) ? 1 : 0;
}

int btn_check_c()
{
  volatile int *sw_ptr = (int *)IO_C_ADDR;
  return (*sw_ptr & 0x1) ? 1 : 0;
}
