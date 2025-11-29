#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//testing libraries
#include <stdint.h>
#include <unistd.h>
//attiny headers
//#include <avr/io.h>
//#include <util/delay.h>


//All times in ms
#define DOT 1000
#define DASH 3000
#define LETTER_SEPERATION 3000
#define WORD_SEPERATION 7000
#define TOL 500

#define MAX_LETTER_LENGTH 4
#define MAX_MESSAGE_LENGTH 20

//morse table for decoding
const char* morse_table[] =
{
        "    ", //" " (space)
        ".-  ", //A
        "-...", //B
        "-.- ", //C
        "-.. ", //D
        ".   ", //E
        "..-.", //F
        "--. ", //G
        "....", //H
        "..  ", //I
        ".-- ", //J
        "-.- ", //K
        ".-..", //L
        "--  ", //M
        "-.  ", //N
        "--- ", //O
        ".--.", //P
        "--.-", //Q
        ".-. ", //R
        "... ", //S
        "-   ", //T
        "..- ", //U
        "...-", //V
        ".-- ", //W
        "-..-", //X
        "-.--", //Y
        "--..", //Z
};

//Includes a space at the start
const char alphabet[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char *message = calloc(MAX_MESSAGE_LENGTH+1,sizeof(char));
char *current_letter = calloc(MAX_LETTER_LENGTH+1,sizeof(char));


void interpret_buttons() {
        if(button_pressed == 2) {

                if(b2_press_time>LETTER_SEPERATION) {
                        if(b2_press_time>WORD_SEPERATION) {
                                current_letter = "    ";
                        }
                        update_string(message,MAX_MESSAGE_LENGTH,decode(current_letter));
                }
        }
        else if(button_pressed == 0) {
                if(abs(b2_press_time-DOT) < TOL)
                        update_string(current_letter,MAX_LETTER_LENGTH,'.');
                else if(abs(b2_press_time-DASH) < TOL)
                        update_string(current_letter,MAX_LETTER_LENGTH,'-');
        }
        else if(button_pressed == 1) {
                send_data();
                message = "";
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


char decode_letter()
{
	for (int i = 0; i < 27; i++)
	{
                if(!strncmp(*(morse_table+i), current_letter, MAX_LETTER_LENGTH))
                        return *(alphabet+i);
	}
}