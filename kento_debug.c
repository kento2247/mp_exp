// デバイスの初期化を行う。必ずcrt0.cを最初にincludeする
#include "crt0.c"

// ハードウェア操作用のプログラム
#include "button.c"
#include "game.c"
#include "handler_func.c"
#include "lcd.c"
#include "tone.c"

// 定数の宣言
#define MAX_UINT 4294967295
#define HANDLER_INTERVAL 10 // 10msec

// 変数の宣言

// 関数のプロトタイプ宣言
void interrupt_handler();
void setup();
void loop();
void handler_counter();
void sleep(unsigned int msec);

// グローバル変数

// 割り込みハンドラ
// interrupt_handler() is called every 10msec (HANDLER_INTERVAL msec)
void interrupt_handler()
{
  if (enable_interrupt == 0)
    return;          // 割り込みが無効なら何もしない
  handler_counter(); // ハンドラカウンタを更新する
  game_demo();
}

// 初期設定
void setup()
{
  lcd_init(); // LCDの初期化
  lcd_str("setup");
  tone_demo();
  lcd_str("tone demo end");
  enable_interrupt = 1;
}

// メインループ
void loop()
{
  btn_states_update(); // ボタンの状態を更新する
}

// メイン関数
int main()
{
  setup();
  while (1)
    loop();
}