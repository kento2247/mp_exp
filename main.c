// タイトル：リズムテニスゲーム
// アピールポイント
// 乱数は使っていない
// 5班
// デバイスの初期化を行う。必ずcrt0.cを最初にincludeする
#include "crt0.c"

// ハードウェア操作用のプログラム
#include "button.c"
#include "game.c"
#include "handler_func.c"
#include "lcd.c"
#include "tone.c"

// 関数のプロトタイプ宣言
void interrupt_handler();
void setup();
void loop();
void main();
// グローバル変数

// 割り込みハンドラ
// interrupt_handler() is called every 10msec (HANDLER_INTERVAL msec)
void interrupt_handler()
{
  handler_counter(); // ハンドラカウンタを更新する
  if (enable_interrupt == 0)
    return; // 割り込みが無効なら何もしない

  switch (game_state)
  {
  case 0:
    game_init();
    int i;
    for (i = 0; i < 10; i++)
    {
      tone_play(4);
    }
    game_opening();
    game_state = 1;
    break;
  case 1:
    if (life[0] == 0 || life[1] == 0)
      game_state = 2; // gameを終了し、次の状態へ遷移
    else
      game_play();
    break;
  case 2:
    game_ending((life[0] == 0) ? 1 : 0);
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
    break;
  case 3:
    lcd_clear();
    break;
  }
}

// 初期設定

void setup()
{
  lcd_init();
  // lcd_demo_animation();
  // tone_demo();
  enable_interrupt = 1; // 割り込みを有効にする（開始）
}
// メインループ
void loop()
{
  btn_states_update(); // ボタンの状態を更新する(button.c)
}

// メイン関数
void main()
{
  setup();
  while (1)
  {
    loop();
    if (game_state == 3)
    {
      break;
    }
  }
}