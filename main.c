// デバイスの初期化を行う。必ずcrt0.cを最初にincludeする
#include "crt0.c"

// ハードウェア操作用のプログラム
#include "button.c"
#include "lcd.c"
#include "tone.c"

// 定数の宣言
#define MAX_UINT 4294967295
#define HANDLER_INTERVAL 100  // 100msec

// 関数のプロトタイプ宣言
void main();
void setup();
void loop();
void interrupt_handler();
void sleep(unsigned int msec);

// グローバル変数
unsigned int handler_cnt = 0;

// 割り込みハンドラ
void interrupt_handler() {
  // interrupt_handler() is called every 100msec (HANDLER_INTERVAL msec)
  handler_cnt++;
  if (handler_cnt >= MAX_UINT) handler_cnt = 0;
}

// メイン関数
void main() {
  setup();
  while (1) loop();
}

// 初期設定
void setup() {}

// メインループ
void loop() {
  btn_states_update();  // ボタンの状態を更新する
}

void sleep(unsigned int msec) {
  // 100msec単位でsleepする関数
  unsigned int start = handler_cnt;
  unsigned int add_time = msec / HANDLER_INTERVAL;
  unsigned int laxucity = MAX_UINT - start;  // オーバーフローまでの猶予
  unsigned int end;
  if (laxucity < add_time) {
    unsigned int difference = add_time - laxucity;
    while (handler_cnt < MAX_UINT)
      ;
    end = difference;
  } else
    end = start + add_time;
  while (handler_cnt < end) {
    // nop
    ;
  }
}