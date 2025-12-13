/*
Benjamin Mock
Created 28 November 2025
This is the main logic code for the capstone project that will run on an ATTINY84 and meant to be a simple communication device.
*/
#include "capstone_input.h"
#include "capstone_morse.h"
#include "capstone_display.h"
#include "wrapper.h"

#define STARTUP_DELAY 20000


int main(void) {

    //This enables interrupts and starts the timers, so it must always fun first
    master_button_init();


    //programming delay
    uint32_t a = millis_now();
    uint32_t b = a;
    while(b - a < STARTUP_DELAY) {b = millis_now();}

    //I2C init
    twm_init();

    oled_init(); //Send SSD1306 init sequence
    oled_clear();
    oled_set_cursor(0, 0);

    //I2C LED test
//    led_i2c_test();

    //Display test
    test1();
    while(1);

    while (1) {
        read_buttons();
        interpret_buttons();
    }
}

