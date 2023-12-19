#pragma once

#include "handler_func.c"

// #define IO_A_ADDR 0xff1a  // between display and speaker
#define IO_B_ADDR 0xff10  // speaker
// #define IO_C_ADDR 0xff14  // under the display
// #define BTN_ADDR 0xff04

// 関数のプロトタイプ宣言
void tone_play(int id);
void tone_play_time(int id, int wait_msec);
void tone_wait(int n);

void tone_demo() {
  tone_play_time(5, 200);
  tone_play_time(0, 300);
  tone_play_time(5, 200);
  tone_play_time(0, 300);
  tone_play_time(5, 200);
  tone_play_time(6, 200);
  tone_play_time(8, 200);
}

void tone_wait(int n) {
  // 3000 ~= 1msec。nクロック待つ
  int i;
  for (i = 0; i < n; i++)
    ;
}

void tone_play_time(int id, int wait_msec) {
  tone_play(id);
  // tone_wait(wait_msec * 3000);
  handler_sleep(wait_msec);
  tone_play(0);
}

void tone_play(int id) {
  // (mode == 1) ? 14931 * 2 : /* C */
  //     (mode == 2) ? 14093 * 2
  //                 : /* C# */
  //     (mode == 3) ? 13302 * 2
  //                 : /* D */
  //     (mode == 4) ? 12555 * 2
  //                 : /* D# */
  //     (mode == 5) ? 11850 * 2
  //                 : /* E */
  //     (mode == 6) ? 11185 * 2
  //                 : /* F */
  //     (mode == 7) ? 10558 * 2
  //                 : /* F# */
  //     (mode == 8) ? 9965 * 2
  //                 : /* G */
  //     (mode == 9) ? 9406 * 2
  //                 : /* G# */
  //     (mode == 10) ? 8878 * 2
  //                  : /* A */
  //     (mode == 11) ? 8380 * 2
  //                  : /* A# */
  //     (mode == 12) ? 7909 * 2
  //                  : /* B */
  //     (mode == 13) ? 7465 * 2
  //                  : /* C */
  //     if (id < 0 || id > 13) return;
  volatile int *iob_ptr = (int *)IO_B_ADDR;
  *iob_ptr = id;
}