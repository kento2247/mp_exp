#pragma once

#include "handler_func.c"

// #define IO_A_ADDR 0xff1a  // between display and speaker
#define IO_B_ADDR 0xff10 // speaker
// #define IO_C_ADDR 0xff14  // under the display
// #define BTN_ADDR 0xff04

void tone_play(int id)
{
  int len;
  volatile int *iob_ptr = (int *)IO_B_ADDR;
  switch (id)
  {
  case 1:
    len = 300000;
    break;
  case 2:
    len = 50000;
    break;
  case 3:
    len = 10000;
    break;
  case 4:
    len = 9949;
    break;
  case 5:
    len = 8880;
    break;
  case 6:
    len = 7891;
    break;
  case 7:
    len = 7029;
    break;
  case 8:
    len = 6639;
    break;
  }
  *iob_ptr = 1;
  lcd_wait(len);
  *iob_ptr = 0;
  lcd_wait(len);
}