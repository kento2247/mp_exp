#pragma once

// 依存ファイルのインクルード
#include "button.c"
#include "handler_func.c"
#include "lcd.c"

// 関数のプロトタイプ宣言
void game_demo();
void game_opening();

// ゲームの状態を表す変数
int game_state = 0; // 0: opening, 1: playing, 2: ending
int ball_index = 0; // ボールの位置。左が0、maxはDISPLAY_COL
int life[2] = {0, 0};

void game_demo()
{
  lcd_clear();
  lcd_str("demo: ");
  if (btn_get_state(0))
  {
    lcd_str("Button 0");
  }
  else if (btn_get_state(1))
  {
    lcd_str("Button 1");
  }
  else if (btn_get_state(2))
  {
    lcd_str("Button 2");
  }
  else if (btn_get_state(3))
  {
    lcd_str("Button 3");
  }
  else if (btn_get_state(4))
  {
    lcd_str("Button A");
  }
  else if (btn_get_state(5))
  {
    lcd_str("Button C");
  }

  if (handler_cnt % 5000 > 4995)
  {
    lcd_clear();
    lcd_str("handler_cnt > 4995");
    handler_sleep(100);
  }
}

void game_init()
{
  int i = 0;
  for (i = 0; i < 2; i++)
    life[i] = 0;
  ball_index = 0;
  game_state = 0;
}

void game_opening()
{
  lcd_str("Press any button");
  while (1)
  {
    if (btn_states[0] || btn_states[1] || btn_states[2] || btn_states[3] ||
        btn_states[4] || btn_states[5])
    {
      game_state = 1;
      break;
    }
  }
  lcd_str("Game start!");
}

void game_ending(int winner)
{
  unsigned char data[DISPLAY_ROW][DISPLAY_COL] = {
      "game end.", "winner player is", "", "press button to end"};
  if (winner == 0)
    data[DISPLAY_ROW - 2][0] = 'a';
  else if (winner == 1)
    data[DISPLAY_ROW - 2][0] = 'c';
  else
    data[DISPLAY_ROW - 2][0] = ' ';
}