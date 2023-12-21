#pragma once

// 依存ファイルのインクルード
#include "button.c"
#include "handler_func.c"
#include "lcd.c"

// 関数のプロトタイプ宣言
void game_opening();
void game_ending(int winner);
void game_init();
void game_play();
void game_show_ball();
void game_judge();
unsigned char intToChar(int num);

// ゲームの状態を表す変数
int game_state = 0;  // 0: opening, 1: playing, 2: ending
int life[2] = {5, 5};
int virturl_index;  // ゲーム判定をするための仮想的なx軸の長さ:
                    // 0~200（実際にはlen=20マス）
int move_direction = 1;  // ボールの移動方向。1: 増加, -1: 減少
int just_flag = 0;  // 0: 通常, 1: just。球速を変化させる条件に使用
int play_stop_flag =
    0;  // deadline missを検知したら1にする。btmを押してゲーム再開

void game_init() {
  int i = 0;
  for (i = 0; i < 2; i++) life[i] = 5;
  game_state = 0;
  virturl_index = 0;
  move_direction = 1;
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

void game_show_ball(int ball_index) {
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
  if (btn_get_state(4)) {
    if ((virturl_index > 170 && virturl_index <= 180)) {
      just_flag = 0;
    } else if ((virturl_index > 180 && virturl_index <= 190)) {
      just_flag = 1;
    } else if ((virturl_index > 190 && virturl_index <= 200)) {
      just_flag = 0;
    } else
      move_direction = !move_direction;  // ボールの移動方向を反転
    move_direction = !move_direction;    // ボールの移動方向を反転
  } else if (btn_get_state(5)) {
    if ((virturl_index >= 20 && virturl_index < 30)) {
      just_flag = 0;
    } else if ((virturl_index >= 10 && virturl_index < 20)) {
      just_flag = 1;
    } else if ((virturl_index >= 0 && virturl_index < 10)) {
      just_flag = 0;
    } else
      move_direction = !move_direction;  // ボールの移動方向を反転
    move_direction = !move_direction;    // ボールの移動方向を反転
  }

  if (!play_stop_flag) {  // 通常モード
    if (just_flag == 0) {
      if (move_direction == 1)
        virturl_index++;
      else
        virturl_index--;
    } else if (just_flag == 1) {
      // 倍速モード
      if (move_direction == 1)
        virturl_index = virturl_index + 2;
      else
        virturl_index = virturl_index - 2;
    }
  }
  if (virturl_index > 200) {
    life[1]--;  // Cはdeadline miss
    play_stop_flag = 1;
    virturl_index = 200;
    move_direction = -1;  // ボールの移動方向を左へ
  } else if (virturl_index < 0) {
    virturl_index = 0;
    life[0]--;  // Aはdeadline miss
    play_stop_flag = 1;
    virturl_index = 0;
    move_direction = 1;  // ボールの移動方向を右へ
  }
  game_show_ball(virturl_index / 10);
}

unsigned char intToChar(int num) {
  unsigned char res = '0' + num;
  return res;
}