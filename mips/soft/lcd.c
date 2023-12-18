#define DISPLAY_ROW 4
#define DISPLAY_COL 20

// LCD 制御関数のプロトタイプ宣言
void lcd_wait(int n);
void lcd_cmd(unsigned char cmd);
void lcd_init();
void lcd_update(unsigned char data[DISPLAY_ROW][DISPLAY_COL]);
void lcd_char(unsigned char data);
void lcd_str(unsigned char *str);

// LCD アニメーション関数
void animate() {
  int i, j;
  unsigned char data[DISPLAY_ROW][DISPLAY_COL] = {
      " Hello, World!", "               ", "               ",
      "               "};

  lcd_init();
  lcd_update(data);
}

// 以下は仮の実装です。実際の環境に合わせて LCD 制御関数を実装してください。

void lcd_wait(int n) {
  int i;
  for (i = 0; i < n; i++)
    ;
}

void lcd_str(unsigned char *str) {
  int i = 0;
  for (i = 0; i < DISPLAY_COL; i++) {
    lcd_char(*str++);
  }
}

void lcd_char(unsigned char data) {
  /* E, RS, RW, DB[7:0] */
  volatile int *lcd_ptr = (int *)0xff0c;
  *lcd_ptr = (0x000000ff & data) | 0x00000200; /* E=0,RS=1,RW=0 */
  lcd_wait(1);
  *lcd_ptr = (0x000000ff & data) | 0x00000600; /* E=1,RS=1,RW=0 */
  lcd_wait(2);
  *lcd_ptr = (0x000000ff & data) | 0x00000200; /* E=0,RS=1,RW=0 */
  lcd_wait(278);
}

void lcd_cmd(unsigned char cmd) {
  /* E, RS, RW, DB[7:0] */
  volatile int *lcd_ptr = (int *)0xff0c;
  *lcd_ptr = (0x000000ff & cmd) | 0x00000000; /* E=0,RS=0,RW=0 */
  lcd_wait(1);
  *lcd_ptr = (0x000000ff & cmd) | 0x00000400; /* E=1,RS=0,RW=0 */
  lcd_wait(2);
  *lcd_ptr = (0x000000ff & cmd) | 0x00000000; /* E=0,RS=0,RW=0 */
  lcd_wait(11389);
}

void lcd_init() {
  lcd_wait(104167);
  lcd_cmd(0x38); /* 8-bit, 2-line mode */
  lcd_cmd(0x06); /* Cursor auto increment */
  lcd_cmd(0x0c); /* Display ON */
  lcd_cmd(0x01); /* Clear display */
  lcd_wait(10417);
}

void lcd_update(unsigned char data[DISPLAY_ROW][DISPLAY_COL]) {
  int i;
  int pos;  // address
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
