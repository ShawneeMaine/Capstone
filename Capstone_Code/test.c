/*
Benjamin Mock
Created 29 November 2025
This is a testing file to simulate button push inputs.
*/
#include "input_simulation.h"
#include "capstone_morse.h"
#include "capstone_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
    uint32_t button_value;
    char input[32];

    while (1) {
        printf("Enter a duration value (ms)\n");

        if (fgets(input, sizeof(input), stdin) != NULL) {
            //Try to convert to integer
            button_value = (uint32_t) strtoul(input, NULL, 10);

            printf("You entered: %u\n", button_value);

            last_change_b2 = button_value;
            deb_flag = 1;
            read_buttons();
            if(button_pressed == 0)
                printf("Button is currently released\n");
            else if(button_pressed == 2)
                printf("Button is currently pressed\n");
            printf("Duration of prevous event is %d\n\n\n",b2_press_time);
            interpret_buttons();
        }
    }
}

