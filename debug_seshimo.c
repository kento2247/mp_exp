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
void handler_counter();
void sleep(unsigned int msec);

// グローバル変数
int pos;
int cnt;
int just_flag = 0;

void show_ball(int pos)
{
    lcd_cmd(0x01); /* Clear display */
    lcd_cmd(0xc0 + pos);
    lcd_data('o');
}

// 割り込みハンドラ
// interrupt_handler() is called every 10msec (HANDLER_INTERVAL msec)
void interrupt_handler()
{
    if (enable_interrupt == 0)
        return;        // 割り込みが無効なら何もしない
    handler_counter(); // ハンドラカウンタを更新する
    if (game_state == 0)
    {
        cnt = 0;
    }
    if (game_state == 1)
    {
        pos = (cnt < 200) ? cnt / 10 : 39 - cnt / 10;
        show_ball(pos);
        if (just_flag == 0)
        {
            if (++cnt >= 400)
            {
                cnt = 0;
            }
        }
        else if (just_flag == 1)
        {
            cnt = cnt + 2;
            if (cnt >= 400)
            {
                cnt = 0;
            }
        }
    }
}

// 初期設定
void setup()
{
    lcd_init(); // LCDの初期化
    game_init();
    // tone_demo();
    enable_interrupt = 1; // 割り込みを有効にする（開始）
    game_state = 1;
}

void play()
{
    while (1)
    {
        /* Button0 is pushed when the ball is in the left edge */

        if ((cnt >= 360 && cnt <= 369) && btn_check_a())
        {
            cnt = 30;
            just_flag = 0;
        }
        else if ((cnt >= 370 && cnt <= 379) && btn_check_a())
        {
            cnt = 20;
            just_flag = 1;
        }
        else if ((cnt >= 380 && cnt <= 389) && btn_check_a())
        {
            cnt = 10;
            just_flag = 0;
        }

        else if ((cnt >= 160 && cnt <= 169) && btn_check_a())
        {
            cnt = 230;
            just_flag = 0;
        }
        else if ((cnt >= 170 && cnt <= 179) && btn_check_a())
        {
            cnt = 220;
            just_flag = 1;
        }
        else if ((cnt >= 180 && cnt <= 189) && btn_check_a())
        {
            cnt = 210;
            just_flag = 0;
        }
    }
}
// メインループ
void loop()
{
    btn_states_update(); // ボタンの状態を更新する(button.c)
    play();
}

// メイン関数
void main()
{
    setup();
    while (1)
        loop();
}