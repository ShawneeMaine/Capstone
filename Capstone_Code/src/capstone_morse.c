#include "capstone_morse.h"

//For testing purposes only the real header is replaced with the simulation header
//#include "input_simulation.h"

#include "capstone_input.h"
#include "capstone_display.h"
#include <stdlib.h>
#include <string.h>
//#include <stdio.h>
//testing libraries
//#include <unistd.h>
//attiny headers
#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PA5
#define LED_DDR DDRA
#define LED_PORT PORTA

//morse table for decoding
const char* morse_table[] =
{
        " ", //" " (space)
        ".-", //A
        "-...", //B
        "-.-.", //C
        "-..", //D
        ".", //E
        "..-.", //F
        "--.", //G
        "....", //H
        "..", //I
        ".---", //J
        "-.-", //K
        ".-..", //L
        "--", //M
        "-.", //N
        "---", //O
        ".--.", //P
        "--.-", //Q
        ".-.", //R
        "...", //S
        "-", //T
        "..-", //U
        "...-", //V
        ".--", //W
        "-..-", //X
        "-.--", //Y
        "--..", //Z
};

//Includes a space at the start
const char alphabet[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char message[MAX_MESSAGE_LENGTH + 1];
char current_letter[MAX_LETTER_LENGTH + 1];

void interpret_buttons() {
        if(button_pressed == 2) {
		LED_DDR |= (1<<LED_PIN);
		LED_PORT |= (1<<LED_PIN);
                if((b2_press_time>LETTER_SEPERATION) || (strlen(current_letter) == MAX_LETTER_LENGTH)) {
                        update_string(message,MAX_MESSAGE_LENGTH,decode());
                        if(b2_press_time>WORD_SEPERATION) {
                                strcpy(current_letter, " ");
                                update_string(message,MAX_MESSAGE_LENGTH,decode());
                        }
                        memset(current_letter,0,sizeof(current_letter));
                }
        }
        else if(button_pressed == 0) {
		LED_PORT &= ~(1<<LED_PIN);
                if(abs(b2_press_time-DOT) < TOL)
                        update_string(current_letter,MAX_LETTER_LENGTH,'.');
                else if(abs(b2_press_time-DASH) < TOL)
                        update_string(current_letter,MAX_LETTER_LENGTH,'-');
        }
        else if(button_pressed == 1) {
//              send_data();
                memset(message, 0, sizeof(message));
                memset(current_letter,0,sizeof(current_letter));
        }
        update_display();
}


//function to append letter to buffer
void update_string(char* str, uint8_t max_len, char letter)
{
        uint8_t len = strlen(str);
        if(len == max_len)
                return;
	*(str+len) = letter;
}


char decode()
{
        uint8_t i;
	for (i = 0; i < 27; i++)
	{
                if(strlen(current_letter) != strlen(morse_table[i]))
                        continue;
                if(!strncmp(*(morse_table+i), current_letter, MAX_LETTER_LENGTH)) {
                        return *(alphabet+i);
                }
	}
        return -1;
}
