/*
Benjamin Mock
Created 28 November 2025
This is the main logic code for the capstone project that will run on an ATTINY84 and meant to be a simple communication device.
*/
#include "capstone_input.h"
#include "capstone_display.h"
#include "wrapper.h"
#include "uart.h"
#include <string.h>

#include <avr/wdt.h>

#define STARTUP_DELAY 20000
#define LED_PORT PORTA
#define LED_PIN PA5


int main(void) {

    MCUSR = 0;
    wdt_disable();
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
    //test1();

    //UART init
    softuart_init();
    //transmit_test();
    uint8_t msg_index;

    while (1) {
        if(softuart_rx_available()) {
            msg_index = 0;
            memset(msg, 0, sizeof(msg));
            while (softuart_rx_available()) {
                uint8_t c = softuart_rx_read();
                // Append to message if there is space
                if (msg_index < sizeof(msg) - 1) {
                    msg[msg_index++] = c;
                    msg[msg_index] = '\0'; // keep null-terminated
                }
            }
        update_display();
        }
        // Update display with whatever has been received so far
        read_buttons();
    }
}

