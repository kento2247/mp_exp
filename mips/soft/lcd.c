/*
 * LCD functions
 */

#define DISPLAY_ROW 4
#define DISPLAY_COL 20

void lcd_wait(int n) {
  int i;
  for (i = 0; i < n; i++)
    ;
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

void lcd_update(unsigned char *data) {
  int i, j;
  lcd_init();
  for (i = 0; i < DISPLAY_ROW; i++) {
    lcd_cmd(0x80 + i * 0x40);
    for (j = 0; j < DISPLAY_COL; j++) {
      lcd_data(data[i * DISPLAY_COL + j]);
    }
  }
}

void demo() {
  unsigned char data[DISPLAY_ROW][DISPLAY_COL] = {
      "Hello World!", "Hello World!", "Hello World!", "Hello World!"};
  lcd_update(&data);
}

/*
#include "lcd.c"

#include "crt0.c"

void interrupt_handler() {}
int main() { demo(); }
*/