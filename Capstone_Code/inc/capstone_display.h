#ifndef CAPSTONE_DISPLAY_H
#define CAPSTONE_DISPLAY_H

#include <stdint.h>
void update_display();
void display_message();
void test1();
void oled_send_command(uint8_t cmd);
void oled_send_data(const uint8_t *data, uint8_t len);
void oled_set_cursor(uint8_t page, uint8_t column);
void oled_write_char6(const uint8_t data[6]);
void oled_clear(void);
void oled_init(void);


#endif