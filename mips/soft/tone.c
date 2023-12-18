void tone_wait(int n)
{
  // 3000 ~= 1msec
  int i;
  for (i = 0; i < n; i++)
    ;
}

void tone_play_time(int id, int wait_time)
{
  if (id < 0 || id > 13)
    return;
  volatile int *iob_ptr = (int *)0xff10;
  *iob_ptr = id;
  tone_wait(wait_time);
  *iob_ptr = 0;
}

void tone_play(int id)
{
  if (id < 0 || id > 13)
    return;
  volatile int *iob_ptr = (int *)0xff10;
  *iob_ptr = id;
  *iob_ptr = 0;
}