#ifndef CAPSTONE_INPUT_H
#define CAPSTONE_INPUT_H


//The libraries used to program the ATTINY84
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUTTON1_PIN PA2
#define BUTTON2_PIN PA3

//The debounce time in milliseconds
#define DEBOUNCE_MS 20
uint32_t millis_now();
void timer0_init();
void io_init();
void master_button_init();
void read_buttons();

#endif