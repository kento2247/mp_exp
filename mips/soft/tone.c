// #define IO_A_ADDR 0xff1a  // between display and speaker
#define IO_B_ADDR 0xff10  // speaker
// #define IO_C_ADDR 0xff14  // under the display
// #define BTN_ADDR 0xff04

void tone_wait(int n) {
  // 3000 ~= 1msec
  int i;
  for (i = 0; i < n; i++)
    ;
}

void tone_play_time(int id, int wait_time) {
  tone_play(id);
  tone_wait(wait_time);
  tone_play(0);
}

void tone_play(int id) {
  if (id < 0 || id > 13) return;
  volatile int *iob_ptr = (int *)IO_B_ADDR;
  *iob_ptr = id;
}