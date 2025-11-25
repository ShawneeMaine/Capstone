 #include <stdlib.h>
#include <string.h>
#include <stdio.h>
//attiny headers
#include <avr/io.h>
#include <util/delay.h>
//to account for 10ms delay, each value is its actual time in ms/10
#define DOT 100
#define DASH 300

//defining buffer for storing the current message to be sent
char *message = NULL;
char *current_letter = NULL;
size_t len = 0;

//function to append letter to buffer
void append_to_char(char c, char* message)
{
        message = realloc(message, len+1);
        message[len] = c;
        len++;
}

//morse table for decoding
const char* morse_table[] =
{
        ".-00", //A
        "-...", //B
        "-.-0", //C
        "-..0", //D
        ".000", //E
        "..-.", //F
        "--.0", //G
        "....", //H
        "..00", //I
        ".--0", //J
        "-.-0", //K
        ".-..", //L
        "--00", //M
        "-.00", //N
        "---0", //O
        ".--.", //P
        "--.-", //Q
        ".-.0", //R
        "...0", //S
        "-000", //T
        "..-0", //U
        "...-", //V
        ".--0", //W
        "-..-", //X
        "-.--", //Y
        "--..", //Z
};

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char decode_letter(char* letter)
{
	for (int i = 0; i < 26; i++)
	{
		if (strncmp(letter, morse_table[i], 4) == 0)
		{
			return alphabet[i];
		}
	}
	printf("Decoding failed on input %c.\n", *letter);
	return -1;
}

int main()
{
        //set pins 10 and 11 to be input. high when button not pressed, low when button is pressed
        DDRA &= ~(1<<PA0)|~(1<<PA1);
        //enable internal pullup resistors
        PORTA |= (1<<PA0)|(1<<PA1);
        //counters for morse code
        uint8_t press_duration = 0;
        uint8_t release_duration = 0;

        while(1)
        {
                //read buttons
                uint8_t morse_b = PINA & (1<<PA0);
                uint8_t send_b = PINA & (1<<PA1);
                int dot_or_dash = -1;

                if (send_b == 0) //checking for if send button is pressed
                {
                        //send message to other device
                        //reset
                        press_duration = 0;
                        release_duration = 0;
                        current_letter = NULL;
                        message = NULL;
                        len = 0;
                }
                if (morse_b == 0)//if morse button is pressed
                {
                        //count how long button is being pressed for
                        press_duration++;

                        if (release_duration >= 7 * DOT)//new word
                        {
                                //append last character, append space character, reset current letter array
                                //resetting values
                                current_letter = NULL;
                                release_duration = 0;
                        }
                        else if ((DOT - 25 < release_duration) && (release_duration < DOT + 25)) //if delay is around 3s, that's the end of the letter. 250ms leniency
                        {
                                //append letter to message, reset current letter array
                                //resetting values
                                current_letter = NULL;
                                release_duration = 0;
                        }
                }
                else //if morse button is not pressed
                {
                        //if we are in this code block, then morse_b is unpressed, and we need to count the length of release
                        release_duration++;

                        //checking for dot or dash
                        if ((DOT - 25 < press_duration) && (press_duration < DOT + 25))//check for dot, 250ms of leniency
                        {
                                dot_or_dash = 0;
                        }
                        else if ((DASH - 25 < press_duration) && (press_duration < DASH + 25))//check for dash, 250ms of leniency
                        {
                                dot_or_dash = 1;
                        }
                        else
                        {
                                continue;
                        }

                        //updates array for current letter from last button press
                        if (current_letter[3] != 0)//ensuring no oob indexing
                        {
                                if (dot_or_dash == 0)
                                {
                                        append_to_char('.', current_letter);
                                }
                                else if (dot_or_dash == 1);
                                {
                                        append_to_char('-', current_letter);
                                }
                        }
                        //resetting necessary data
                        press_duration = 0;
                        dot_or_dash = -1;
                }
                //due to this delay, the duration counter should be equivalent to number of ms/10
                _delay_ms(10);//debouncing delay

        }
}
