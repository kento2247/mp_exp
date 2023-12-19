#pragma once

// 定数の宣言
#define DISPLAY_ROW 4
#define DISPLAY_COL 20
#define LCD_ADDR 0xff0c

// 変数の宣言

// LCD 制御関数のプロトタイプ宣言
void lcd_demo_animation();
void lcd_data(unsigned char data);
void lcd_wait(int n);
void lcd_cmd(unsigned char cmd);
void lcd_init();
void lcd_update(unsigned char data[DISPLAY_ROW][DISPLAY_COL]);
void lcd_char(unsigned char data);
void lcd_str(unsigned char *str);
void lcd_digit3(unsigned int val);
void lcd_customchar(unsigned int addr, unsigned int *bitmap);
void lcd_clear();

// LCD アニメーション関数
void lcd_demo_animation() {
  lcd_clear();
  unsigned char data[DISPLAY_ROW][DISPLAY_COL] = {
      " Hello, World.      ", " Hello, World.      ", " Hello, World.      ",
      " Hello, World.      "};
  int i, j;
  for (i = 0; i < DISPLAY_COL; i++) {
    for (j = 0; j < DISPLAY_ROW; j++) {
      data[j][i] = ' ';
    }
    lcd_update(data);
    lcd_wait(3000 * 1000);
  }
}

void lcd_wait(int n) {
  int i;
  for (i = 0; i < n; i++)
    ;
}

void lcd_cmd(unsigned char cmd) {
  /* E, RS, RW, DB[7:0] */
  volatile int *lcd_ptr = (int *)LCD_ADDR;
  *lcd_ptr = (0x000000ff & cmd) | 0x00000000; /* E=0,RS=0,RW=0 */
  lcd_wait(1);
  *lcd_ptr = (0x000000ff & cmd) | 0x00000400; /* E=1,RS=0,RW=0 */
  lcd_wait(2);
  *lcd_ptr = (0x000000ff & cmd) | 0x00000000; /* E=0,RS=0,RW=0 */
  lcd_wait(11389);
}

void lcd_data(unsigned char data) {
  /* E, RS, RW, DB[7:0] */
  volatile int *lcd_ptr = (int *)LCD_ADDR;
  *lcd_ptr = (0x000000ff & data) | 0x00000200; /* E=0,RS=1,RW=0 */
  lcd_wait(1);
  *lcd_ptr = (0x000000ff & data) | 0x00000600; /* E=1,RS=1,RW=0 */
  lcd_wait(2);
  *lcd_ptr = (0x000000ff & data) | 0x00000200; /* E=0,RS=1,RW=0 */
  lcd_wait(278);
}

void lcd_init() {
  lcd_wait(104167);
  lcd_cmd(0x38); /* 8-bit, 2-line mode */
  lcd_cmd(0x06); /* Cursor auto increment */
  lcd_cmd(0x0c); /* Display ON */
  lcd_cmd(0x01); /* Clear display */
  lcd_wait(10417);
}

void lcd_str(unsigned char *str) {
  while (*str != '\0') lcd_data(*str++);
}

void lcd_update(unsigned char data[DISPLAY_ROW][DISPLAY_COL]) {
  int i;
  int pos;  // address
  lcd_clear();
  for (i = 0; i < DISPLAY_ROW; i++) {
    switch (i) {
      case 0:
        pos = 0x80;
        break;
      case 1:
        pos = 0xc0;
        break;
      case 2:
        pos = 0x94;
        break;
      case 3:
        pos = 0xd4;
        break;
    }
    lcd_cmd(pos);
    lcd_str(data[i]);
  }
}

void lcd_digit3(unsigned int val) {
  int digit3, digit2, digit1;
  digit3 = (val < 100) ? ' ' : ((val % 1000) / 100) + '0';
  digit2 = (val < 10) ? ' ' : ((val % 100) / 10) + '0';
  digit1 = (val % 10) + '0';
  lcd_data(digit3);
  lcd_data(digit2);
  lcd_data(digit1);
}

void lcd_customchar(unsigned int addr, unsigned int *bitmap) {
  lcd_cmd((addr << 3) | 0x40); /* Set CGRAM address */
  lcd_data(bitmap[0]);
  lcd_data(bitmap[1]);
  lcd_data(bitmap[2]);
  lcd_data(bitmap[3]);
  lcd_data(bitmap[4]);
  lcd_data(bitmap[5]);
  lcd_data(bitmap[6]);
  lcd_data(0x00); /* Last line is used by cursor */
  lcd_cmd(0x80);  /* Set DDRAM address (write to display) */

  lcd_data(addr);
}

void lcd_clear() { lcd_cmd(0x01); /* Clear display */ }