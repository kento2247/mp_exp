#pragma once

// 依存ファイルのインクルード
#include "button.c"
#include "handler_func.c"
#include "lcd.c"

// 関数のプロトタイプ宣言
void game_demo();
void game_opening();

// ゲームの状態を表す変数
int game_state = 0;  // 0: opening, 1: playing, 2: ending

void game_demo() {
  if (btn_get_state(0)) {
    lcd_str("Button 0 pressed");
  }
  if (btn_get_state(1)) {
    lcd_str("Button 1 pressed");
  }
  if (btn_get_state(2)) {
    lcd_str("Button 2 pressed");
  }
  if (btn_get_state(3)) {
    lcd_str("Button 3 pressed");
  }
  if (btn_get_state(4)) {
    lcd_str("Button A pressed");
  }
  if (btn_get_state(5)) {
    lcd_str("Button C pressed");
  }
}

void game_opening() {
  lcd_str("Press any button");
  while (1) {
    if (btn_states[0] || btn_states[1] || btn_states[2] || btn_states[3] ||
        btn_states[4] || btn_states[5]) {
      game_state = 1;
      break;
    }
  }
  lcd_str("Game start!");
}