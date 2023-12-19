#pragma once
#include "lcd.c"

// 定数の宣言
#define MAX_UINT 4294967295
#define HANDLER_INTERVAL 10 // 10msec

// グローバル変数
int enable_interrupt = 0; // 割り込みを有効にするかどうか
unsigned int handler_cnt = 0;

void handler_counter()
{
  // ハンドラカウンタを更新する
  handler_cnt++;
  if (handler_cnt >= MAX_UINT)
    handler_cnt = 0;
}

void handler_sleep(unsigned int msec)
{
  // 10msec単位でsleepする関数
  unsigned int start = handler_cnt;
  unsigned int add_time = msec / HANDLER_INTERVAL;
  unsigned int laxucity = MAX_UINT - start; // オーバーフローまでの猶予
  unsigned int end;
  if (laxucity < add_time)
  {
    unsigned int difference = add_time - laxucity;
    while (handler_cnt < MAX_UINT)
      ;
    end = difference;
  }
  else
    end = start + add_time;
  while (handler_cnt < end)
  {
    // nop
    lcd_digit3(handler_cnt);
    ;
  }
}