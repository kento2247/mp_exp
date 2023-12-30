#pragma once

/*
 * ボタンの状態を取得する関数群
 * ボタンの状態はbtn_statesに格納される
 * btn_states[0]~btn_states[3]はボタン0~3の状態
 * btn_states[4]はボタンAの状態
 * btn_states[5]はボタンCの状態
 * ボタンが押されているときtrue,通常はfalse
 * ボタンの状態はbtn_states_update()を呼び出すことで更新される
 * btn_states_update()はmain()の中で定期的に呼び出す必要がある
 */

#define IO_A_ADDR 0xff18 // between display and speaker
// #define IO_B_ADDR 0xff10  // speaker
#define IO_C_ADDR 0xff14 // under the display
#define BTN_ADDR 0xff04
#define BUTTON_NUM 6

// 関数のプロトタイプ宣言
int btn_get_state(int button_num);
void btn_states_update();
int btn_check_num(int button_num);
int btn_check_a();
int btn_check_c();

// ボタンの状態を格納する配列
int btn_states[BUTTON_NUM];

// ボタンの状態を取得する
int btn_get_state(int button_num)
{
  int state = btn_states[button_num];
  btn_states[button_num] = 0;
  return state;
}

void btn_wait_any()
{
  while (1)
  {
    btn_states_update();
    if (btn_states[0] || btn_states[1] || btn_states[2] || btn_states[3] ||
        btn_states[4] || btn_states[5])
    {
      break;
    }
  }
  for (int i = 0; i < 6; i++)
  {
    btn_states[i] = 0;
  }
}

void btn_wait_A()
{
  while (1)
  {
    btn_states_update();
    if (btn_states[4])
    {
      break;
    }
  }
  for (int i = 0; i < 6; i++)
  {
    btn_states[i] = 0;
  }
}

void btn_wait_C()
{
  while (1)
  {
    btn_states_update();
    if (btn_states[5])
    {
      break;
    }
  }
  for (int i = 0; i < 6; i++)
  {
    btn_states[i] = 0;
  }
}

// ボタンの状態を更新する
void btn_states_update()
{
  int i;
  for (i = 0; i < 4; i++)
  {
    btn_states[i] = btn_check_num(i);
  }
  btn_states[4] = btn_check_a();
  btn_states[5] = btn_check_c();
}

int btn_check_num(int button_num)
{
  volatile int *btn_ptr = (int *)BTN_ADDR;
  switch (button_num)
  {
  case 0:
    return (*btn_ptr & 0x10) ? 1 : 0;
    break;
  case 1:
    return (*btn_ptr & 0x20) ? 1 : 0;
    break;
  case 2:
    return (*btn_ptr & 0x40) ? 1 : 0;
    break;
  case 3:
    return (*btn_ptr & 0x80) ? 1 : 0;
    break;
  default:
    return 0;
  }
}

int btn_check_a()
{
  volatile int *sw_ptr = (int *)IO_A_ADDR;
  return (*sw_ptr & 0x1) ? 1 : 0;
}

int btn_check_c()
{
  volatile int *sw_ptr = (int *)IO_C_ADDR;
  return (*sw_ptr & 0x1) ? 1 : 0;
}
