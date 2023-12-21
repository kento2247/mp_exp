#pragma once

// 依存ファイルのインクルード
#include "button.c"
#include "handler_func.c"
#include "lcd.c"

// 関数のプロトタイプ宣言
void game_demo();
void game_opening();
void game_ending(int winner);
void game_init();
void game_play();
void game_show_ball();
void u();

// ゲームの状態を表す変数
int game_state = 0;  // 0: opening, 1: playing, 2: ending
int ball_index = 0;  // ボールの位置。左が0、maxはDISPLAY_COL
int life[2] = {5, 5};
int cnt;  // ゲーム判定をするための仮想的なx軸の長さ（実際には20マス）
int just_flag = 0;  // 0: 通常, 1: just。球速を変化させる条件に使用
int play_stop_flag =
    0;  // deadline missを検知したら1にする。btmを押してゲーム再開

void game_demo() {
  lcd_clear();
  lcd_str("demo: ");
  if (btn_get_state(0)) {
    lcd_str("Button 0");
  } else if (btn_get_state(1)) {
    lcd_str("Button 1");
  } else if (btn_get_state(2)) {
    lcd_str("Button 2");
  } else if (btn_get_state(3)) {
    lcd_str("Button 3");
  } else if (btn_get_state(4)) {
    lcd_str("Button A");
  } else if (btn_get_state(5)) {
    lcd_str("Button C");
  }

  if (handler_cnt % 5000 > 4995) {
    lcd_clear();
    lcd_str("handler_cnt > 4995");
    handler_sleep(10);
  }
}

void game_init() {
  int i = 0;
  for (i = 0; i < 2; i++) life[i] = 5;
  ball_index = 0;
  game_state = 0;
}

void game_opening() {
  lcd_clear();

  lcd_cmd(0x80);
  lcd_str("Press any button");

  btn_wait_any();  // buttonが何かしら押されるまで待つ

  lcd_clear();        // 画面をクリアする
  lcd_cmd(0xc0 + 5);  // 左に5マス余白を作る
  lcd_str("Game start!");
  // handler_sleep(10); あんまり意味なかった
}

void game_ending(int winner) {
  unsigned char data[DISPLAY_ROW][DISPLAY_COL] = {
      "game end.", "winner player is", "", "press button to end"};
  if (winner == 0)
    data[DISPLAY_ROW - 2][0] = 'a';
  else if (winner == 1)
    data[DISPLAY_ROW - 2][0] = 'c';
  else
    data[DISPLAY_ROW - 2][0] = ' ';
}

void game_show_ball() {
  lcd_cmd(0x01);               /* Clear display */
  lcd_cmd(0xc0 + ball_index);  // 2列目をボールが往復する
  lcd_data('o');
  lcd_cmd(0xd4);
  lcd_str("HP");
  lcd_cmd(0xd4 + 3);
  lcd_data(intToChar(life[0]));
  lcd_cmd(0xd4 + 15);
  lcd_str("HP");
  lcd_cmd(0xd4 + 19);
  lcd_data(intToChar(life[1]));
}

void game_play() {
  /* Button0 is pushed when the ball is in the left edge */
  if (play_stop_flag == 0) {
    game_judge();
  } else {
    btn_wait_any();
    play_stop_flag = 0;
  }
}

void game_judge() {
  if (btn_check_a()) {
    if ((cnt >= 360 && cnt <= 369)) {
      cnt = 30;
      just_flag = 0;
    } else if ((cnt >= 370 && cnt <= 379)) {
      cnt = 20;
      just_flag = 1;
    } else if ((cnt >= 380 && cnt <= 389)) {
      cnt = 10;
      just_flag = 0;
    } else if (cnt >= 400) {
      life[0]--;  // Aはdeadline miss
      play_stop_flag = 1;
      cnt = 20;
    }
  } else if (btn_check_c()) {
    if ((cnt >= 160 && cnt <= 169)) {
      cnt = 230;
      just_flag = 0;
    } else if ((cnt >= 170 && cnt <= 179)) {
      cnt = 220;
      just_flag = 1;
    } else if ((cnt >= 180 && cnt <= 189)) {
      cnt = 210;
      just_flag = 0;
    } else if (cnt >= 200 && cnt <= 210) {
      life[1]--;  // Cはdeadline miss
      play_stop_flag = 1;
      cnt = 220;
    }
  }

  if (!play_stop_flag) {  // 通常モード
    if (just_flag == 0)
      cnt++;
    else  // 球速を倍にする
      cnt = cnt + 2;
  }
  if (cnt >= 400) {
    cnt = 400;  // 点数をとられてしまうから0に戻す必要はない
  }
  ball_index = (cnt < 200) ? cnt / 10 : 39 - cnt / 10;  // ballの場所を決める
  game_show_ball();
}

unsigned char intToChar(int num) {
  unsigned char res = '0' + num;
  return res;
}