#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//testing libraries
#include <stdint.h>
#include <unistd.h>
//attiny headers
//#include <avr/io.h>
//#include <util/delay.h>
//to account for 10ms delay, each value is its actual time in ms/10
#define DOT 100
#define DASH 300

//defining buffer for storing the current message to be sent
char *message = NULL;
char *current_letter = NULL;
size_t len = 0;

//function to append letter to buffer
void append_to_char(char c, char** message)
{
        *message = realloc(*message, len+1);
        (*message)[len] = c;
        len++;
}

//morse table for decoding
const char* morse_table[] =
{
        ".-", //A
        "-...", //B
        "-.-", //C
        "-..", //D
        ".", //E
        "..-.", //F
        "--.", //G
        "....", //H
        "..", //I
        ".--", //J
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

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char decode_letter(char** letter)
{
	for (int i = 0; i < 26; i++)
	{
		if (strncmp(*letter, morse_table[i], strlen(morse_table[i])) == 0)
		{
			return alphabet[i];
		}
	}
	printf("Decoding failed on input %c.\n", **letter);
	return -1;
}

int main()
{
        //set pins 10 and 11 to be input. high when button not pressed, low when button is pressed
        //DDRA &= ~(1<<PA0)|~(1<<PA1);
        //enable internal pullup resistors
        //PORTA |= (1<<PA0)|(1<<PA1);
        //counters for morse code
        uint16_t press_duration = 0;
        uint16_t release_duration = 0;

        while(1)
        {
                //read buttons
                uint8_t morse_b;//PINA & (1<<PA0);
		printf("0 for button is pressed, 1 for button is unpressed\n");
		scanf("%d", &morse_b);
                uint8_t send_b = 1;//PINA & (1<<PA1);
                int dot_or_dash = -1;

                if (send_b == 0) //checking for if send button is pressed
                {
                        //send message to other device

                        //reset
                        press_duration = 0;
                        release_duration = 0;
			free(current_letter);
			free(message);
                        current_letter = NULL;
                        message = NULL;
                        len = 0;
                }
                if (morse_b == 0)//if morse button is pressed
                {
			//TESTING CODE
			printf("enter duration of button being unpressed\n");
			scanf("%d", &release_duration);

                        //count how long button is being pressed for
                        press_duration++;

                        if (release_duration >= 7 * DOT)//new word
                        {
                                //append last character, append space character
				//append_to_char(decode_letter(&current_letter), &message);
				printf("new word\n");
                        }
                        else if ((DOT - 25 < release_duration) && (release_duration < DOT + 25)) //if delay is around 3s, that's the end of the letter. 250ms leniency
                        {
                                //append letter to message
				append_to_char(decode_letter(&current_letter), &message);
				printf("new letter\n");
                        }
			//resetting values
			free(current_letter);
			current_letter = NULL;
			release_duration = 0;
                }
                else //if morse button is not pressed
                {
			//TESTING CODE
			printf("enter duration of button being pressed\n");
			scanf("%d", &press_duration);

                        //if we are in this code block, then morse_b is unpressed, and we need to count the length of release
                        release_duration++;

                        //checking for dot or dash
                        if ((DOT - 25 < press_duration) && (press_duration < DOT + 25))//check for dot, 250ms of leniency
                        {
                                //dot_or_dash = 0;
				//TESTING CODE
				printf("dot\n");
                        }
                        else if ((275 < press_duration) && (press_duration < 325))//check for dash, 250ms of leniency
                        {
                                //dot_or_dash = 1;
				//TESTING CODE
				printf("dash\n");
                        }
                        else
                        {
				//TESTING CODE
				printf("neither\n");
                                //continue;
                        }

                        //updates array for current letter from last button press
                        /*if (current_letter[3] != 0)//ensuring no oob indexing
                        {
                                if (dot_or_dash == 0)
                                {
                                        append_to_char('.', &current_letter);
                                }
                                else if (dot_or_dash == 1);
                                {
                                        append_to_char('-', &current_letter);
                                }
                        }*/
                        //resetting necessary data
                        press_duration = 0;
                }
                //due to this delay, the duration counter should be equivalent to number of ms/10
                //_delay_ms(10);//debouncing delay
		sleep(1);

        }
}
