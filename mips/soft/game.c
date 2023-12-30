#pragma once

// 依存ファイルのインクルード
#include "button.c"
#include "handler_func.c"
#include "lcd.c"
#include "tone.c"

// 関数のプロトタイプ宣言
void game_opening();
void game_ending(int winner);
void game_init();
void game_play();
void game_show_ball();
void game_judge();
unsigned char intToChar(int num);

// ゲームの状態を表す変数
int game_state = 0; // 0: opening, 1: playing, 2: ending
int life[2] = {5, 5};
int virturl_index;      // ゲーム判定をするための仮想的なx軸の長さ:
                        // 0~200（実際にはlen=20マス）
int move_direction = 1; // ボールの移動方向。1: 増加, -1: 減少
int just_flag = 0;      // 0: 通常, 1: just。球速を変化させる条件に使用
int play_stop_flag =
    0; // deadline missを検知したら1にする。btmを押してゲーム再開
int renda_A_flag = 20;
int renda_B_flag = 20;
int serve_flag = 1;
int critical_flag = 2;

void game_init()
{
  int i = 0;
  for (i = 0; i < 2; i++)
    life[i] = 5;
  game_state = 0;
  virturl_index = 10;
  move_direction = 1;
  renda_A_flag = 20;
  renda_B_flag = 20;
  serve_flag = 1;
  critical_flag = 2;
}

void game_opening()
{
  lcd_clear();

  lcd_cmd(0xc0 + 2);
  lcd_str("Press any button");
  lcd_cmd(0x94 + 5);
  lcd_str("Game start!");

  btn_wait_any(); // buttonが何かしら押されるまで待つ

  lcd_clear(); // 画面をクリアする
}

void game_ending(int winner)
{
  lcd_cmd(0x01);
  lcd_cmd(0x80);

  unsigned char data[DISPLAY_ROW][DISPLAY_COL] = {
      "game end.", "winner player is", "button0 to end", "button1 to restart"};
  /*
  if (winner == 0)
    data[DISPLAY_ROW - 2][5] = 'A';
  else if (winner == 1)
    data[DISPLAY_ROW - 2][8] = 'B';
  else
    data[DISPLAY_ROW - 2][0] = ' ';
  */
  lcd_update(data);
  lcd_cmd(0xc0 + 17);
  if (winner == 0)
  {
    lcd_data('A');
  }
  else if (winner == 1)
  {
    lcd_data('B');
  }
  while (1)
  {
    btn_states_update();
    if (btn_states[0])
    {
      game_state = 3;
      break;
    }
    else if (btn_states[1])
    {
      game_state = 0;
      break;
    }
  }
  for (int i = 0; i < 6; i++)
  {
    btn_states[i] = 0;
  }
}

void game_show_ball(int ball_index)
{
  lcd_cmd(0x01);              /* Clear display */
  lcd_cmd(0xc0 + ball_index); // 2列目をボールが往復する
  lcd_data('o');
  lcd_cmd(0xd4);
  lcd_str("HP");
  lcd_cmd(0xd4 + 3);
  lcd_data(intToChar(life[0]));
  lcd_cmd(0xd4 + 15);
  lcd_str("HP");
  lcd_cmd(0xd4 + 19);
  lcd_data(intToChar(life[1]));

  unsigned int bitmap[7] = {
      0x0E, /* 01110 */
      0x0A, /* 01010 */
      0x0A, /* 01010 */
      0x0E, /* 01110 */
      0x04, /* 00100 */
      0x04, /* 00100 */
      0x04, /* 00100 */
  };

  lcd_customchar1(0x03, bitmap);
  lcd_data(0x03);
  lcd_customchar2(0x07, bitmap);
  lcd_data(0x07);
}

void game_play()
{
  /* Button0 is pushed when the ball is in the left edge */
  if (play_stop_flag == 0)
  {
    renda_A_flag = renda_A_flag + 1;
    renda_B_flag = renda_B_flag + 1;
    game_judge();
  }
  else if (play_stop_flag == 1)
  {
    renda_A_flag = 20;
    renda_B_flag = 20;
    just_flag = 0;
    btn_wait_A();
    play_stop_flag = 0;
    serve_flag = 1;
  }
  else if (play_stop_flag == 2)
  {
    renda_A_flag = 100;
    renda_B_flag = 100;
    just_flag = 0;
    btn_wait_C();
    play_stop_flag = 0;
    serve_flag = 1;
  }
}

void game_judge()
{
  int i;
  if (btn_get_state(4)) // ボタンAが押されたとき
  {
    if ((virturl_index > 170 && virturl_index <= 180 && renda_B_flag >= 20))
    {
      for (i = 0; i < 10; i++)
      {
        tone_play(4);
      }
      just_flag = 0;
      move_direction = -1;
    }
    else if ((virturl_index > 180 && virturl_index <= 190 && renda_B_flag >= 20 && serve_flag == 0))
    {
      for (i = 0; i < 10; i++)
      {
        tone_play(1);
      }
      just_flag = 1;
      move_direction = -1;
      critical_flag++;
    }
    else if ((virturl_index > 180 && virturl_index <= 190 && renda_B_flag >= 20 && serve_flag == 1))
    {
      for (i = 0; i < 10; i++)
      {
        tone_play(4);
      }
      just_flag = 0;
      move_direction = -1;
    }
    else if ((virturl_index > 190 && virturl_index <= 200 && renda_B_flag >= 20))
    {
      for (i = 0; i < 10; i++)
      {
        tone_play(4);
      }
      just_flag = 0;
      move_direction = -1;
    }
    renda_B_flag = 0;
    serve_flag = 0;
  }
  else if (btn_get_state(5)) // ボタンCが押されたとき
  {
    if ((virturl_index >= 20 && virturl_index < 30 && renda_A_flag >= 20))
    {
      for (i = 0; i < 10; i++)
      {
        tone_play(4);
      }
      just_flag = 0;
      move_direction = 1;
    }
    else if ((virturl_index >= 10 && virturl_index < 20 && renda_A_flag >= 20 && serve_flag == 0))
    {
      for (i = 0; i < 10; i++)
      {
        tone_play(8);
      }
      just_flag = 1;
      move_direction = 1;
      critical_flag++;
    }
    else if ((virturl_index >= 10 && virturl_index < 20 && renda_A_flag >= 20 && serve_flag == 1))
    {
      for (i = 0; i < 10; i++)
      {
        tone_play(4);
      }
      just_flag = 0;
      move_direction = 1;
    }
    else if ((virturl_index >= 0 && virturl_index < 10 && renda_A_flag >= 20))
    {
      for (i = 0; i < 10; i++)
      {
        tone_play(4);
      }
      just_flag = 0;
      move_direction = 1;
    }
    renda_A_flag = 0;
    serve_flag = 0;
  }

  if (!play_stop_flag)
  {
    if (just_flag == 0 || serve_flag == 1) // 通常モードかサーブ
    {
      critical_flag = 2;
      virturl_index = virturl_index + move_direction * critical_flag;
    }
    else
    {
      // 倍速モード
      virturl_index = virturl_index + move_direction * critical_flag;
    }
  }

  if (virturl_index > 200)
  {
    move_direction = -1; // ボールの移動方向を左へ
    virturl_index = 189;
    life[1]--; // Cはdeadline miss
    play_stop_flag = 1;
  }
  else if (virturl_index < 0)
  {
    move_direction = 1; // ボールの移動方向を右へ
    virturl_index = 11;
    life[0]--; // Aはdeadline miss
    play_stop_flag = 2;
  }
  game_show_ball(virturl_index / 10);
}

unsigned char intToChar(int num)
{
  unsigned char res = '0' + num;
  return res;
}