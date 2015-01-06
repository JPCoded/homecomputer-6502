#include "lcd.h"
#include "keys.h"
#include "readline.h"

char buffer[81];
char *buffer_pos;
unsigned char last_key = 0xff;
char last_char = 0;

char * readline() {
  buffer_pos = buffer;

  for (;;) {
    keys_update();

    if (keys_get_code() != 0xff) {
      if (last_key == 0xff) {
        last_key = keys_get_code();
        last_char = keys_getc();

        if ((last_key) == KEY_BACKSPACE) {
          if (buffer_pos != buffer) {
            --buffer_pos;
            lcd_goto(lcd_get_x() - 1, lcd_get_y());
            lcd_putc(' ');
            lcd_goto(lcd_get_x() - 1, lcd_get_y());
          }
        } else if (last_char != 0 && buffer_pos - buffer < 39) {
          lcd_putc(last_char);
          if (last_char == '\n') {
            *buffer_pos = '\0';
            break;
          } else {
            *buffer_pos = last_char;
            ++buffer_pos;
          }
        }
      }
    } else {
      last_key = 0xff;
    }
  }

  return buffer;
}