#include "crt0.c"
// 必ずcrt0.cを最初にincludeする

#include "button.c"
#include "lcd.c"
#include "tone.c"

/* Do not remove the following line. Do not remove interrupt_handler(). */

void show_ball(int pos);
void play();
int cnt;

#define INIT 0
#define OPENING 1
#define PLAY 2
#define ENDING 3

int state = INIT, pos = 0;

/* interrupt_handler() is called every 100msec */
void interrupt_handler() {
  if (state == INIT) {
  } else if (state == OPENING) {
    cnt = 0;
  } else if (state == PLAY) {
    /* Display a ball */
    pos = (cnt < 16) ? cnt : 31 - cnt;
    show_ball(pos);
    if (++cnt >= 32) {
      cnt = 0;
    }
  } else if (state == ENDING) {
    end_display();
  }
}

void main() {
  while (1) {
    if (state == INIT) {
      start_display();
      while (!ioc_check_0())  // ボタンが押されるのを待つ(未完成）
      {
        ;
      }
      state = PLAY;
    } else if (state == PLAY) {
      cnt = 0;
      play();
      state = ENDING;
    } else if (state == ENDING) {
      end_display();
      while (~btn_check_my())  // ボタンが押されるのを待つ(未完成）
      {
      }
      state = OPENING;
    }
  }
}

void start_display() {
  lcd_cmd(0x80);
  lcd_str("Game Start");
  lcd_cmd(0xc0);
  lcd_str("Press Button");
}

void end_display() {
  lcd_cmd(0x80);
  lcd_str("Game End");
  lcd_cmd(0xc0);
  lcd_str("Press Button");
}

void play() {
  while (1) {
    /* Button0 is pushed when the ball is in the left edge */

    if (cnt == 28 && ioc_check_0()) {
      cnt = 3;
    } else if (cnt == 29 && ioc_check_0()) {
      cnt = 2;
    } else if (cnt == 30 && ioc_check_0()) {
      cnt = 1;
    }

    else if (cnt == 12 && ioc_check_0()) {
      cnt = 19;
    } else if (cnt == 13 && ioc_check_0()) {
      cnt = 18;
    } else if (cnt == 14 && ioc_check_0()) {
      cnt = 17;
    }

    if (pos == 0 && btn_check_0()) {
      led_blink(); /* Blink LEDs when hit */
                   /* Button3 is pushed when the ball is in the right edge */
    } else if (pos == 15 && btn_check_3()) {
      led_blink(); /* Blink LEDs when hit */
    } else if (btn_check_1()) {
      break; /* Stop the game */
    }
  }
}
void show_ball(int pos) {
  lcd_cmd(0x01);       /* Clear display */
  lcd_cmd(0x80 + pos); /* Set cursor position */
  lcd_data('a');
}
