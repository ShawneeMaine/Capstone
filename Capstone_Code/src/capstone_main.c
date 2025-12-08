/*
Benjamin Mock
Created 28 November 2025
This is the main logic code for the capstone project that will run on an ATTINY84 and meant to be a simple communication device.
*/
#include "usi_i2c.h"
#include "capstone_input.h"
#include "capstone_morse.h"
#include "capstone_display.h"

#define STARTUP_DELAY 20000


int main(void) {
    master_button_init();
    oled_set_cursor(0, 0);

    //programming delay
    uint32_t a = millis_now();
    uint32_t b = a;
    while(b - a < STARTUP_DELAY) {b = millis_now();}

    i2c_init();
    oled_init();

    //testing code:
    test1();
    while(1);

    while (1) {
        read_buttons();
        interpret_buttons();
    }
}

