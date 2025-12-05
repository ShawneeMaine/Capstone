#ifndef CAPSTONE_INPUT_H
#define CAPSTONE_INPUT_H

#include <stdint.h>

//The libraries used to program the ATTINY84
//#include <avr/io.h>
//#include <avr/interrupt.h>

#define BUTTON1_PIN PA2
#define BUTTON2_PIN PA3

//The debounce time in milliseconds
#define DEBOUNCE_MS 20

uint32_t millis_now();
uint32_t get_b1_timestamp();
uint32_t get_b2_timestamp();
void timer0_init();
void io_init();
void master_button_init();
void read_buttons();

extern uint8_t button_pressed;
extern uint32_t b2_press_time;

#endif