/*
Benjamin Mock
Created 28 November 2025
This is the main logic code for the capstone project that will run on an ATTINY84 and meant to be a simple communication device.
*/
#include "capstone_input.h"

int main(void) {
    master_button_init();

    while (1) {
        read_buttons();
        interpret_buttons();
    }
}

