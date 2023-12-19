#pragma once

#include "button.c"
#include "handler_func.c"
#include "lcd.c"

int game_state = 0;  // 0: opening, 1: playing, 2: ending

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