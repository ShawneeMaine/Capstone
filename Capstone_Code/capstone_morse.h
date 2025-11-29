#ifndef CAPSTONE_MORSE_H
#define CAPSTONE_MORSE_H

#include <stdint.h>

//All times in ms
#define DOT 1000
#define DASH 3000
#define LETTER_SEPERATION 3000
#define WORD_SEPERATION 7000
#define TOL 500

#define MAX_LETTER_LENGTH 4
#define MAX_MESSAGE_LENGTH 20

void interpret_buttons();
void update_string(char* str, uint8_t max_len, char letter);
char decode_letter();


#endif