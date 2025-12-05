#ifndef INPUT_SIM_H
#define INPUT_SIM_H

#include <stdint.h>

#define BUTTON1_PIN PA2
#define BUTTON2_PIN PA3

//The debounce time in milliseconds
#define DEBOUNCE_MS 20

uint32_t millis_now();
void timer0_init();
void io_init();
void master_button_init();
void read_buttons();

extern uint8_t button_pressed;
extern uint32_t b2_press_time;

//For testing purposes only:
extern volatile uint32_t last_change_b2;
extern volatile uint8_t deb_flag;

#endif