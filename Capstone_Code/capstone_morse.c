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
#define LENIENCE 25
#define LETTER_LENGTH 4
#define DELAY 10

//function to append letter to buffer
void append_to_string(char c, char** buffer, int *len)
{
	char* temp;
	if ((temp = realloc(*buffer, *len+1)) == NULL)
	{
		printf("memory allocation error, message is too long");
		return;
	}
	*buffer = temp;
        *(*buffer + *len) = c;
	(*len)++;
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

char decode_letter(char* letter)
{
	for (int i = 0; i < 26; i++)
	{
		if (strlen(letter) != strlen(morse_table[i])) continue;
		if (strncmp(letter, morse_table[i], strlen(morse_table[i])) == 0)
		{
			return alphabet[i];
		}
	}
	printf("Decoding failed on input %c.\n", *letter);
	return -1;
}

void reset_string(char **buffer, int *len)
{
	if(*buffer)
	{
		free(*buffer);
		*buffer = calloc(1, sizeof(char));
	}
	*len = 0;
}

int main()
{
        //set pins 10 and 11 to be input. high when button not pressed, low when button is pressed
        //DDRA &= ~(1<<PA0)|~(1<<PA1);
        //enable internal pullup resistors
        //PORTA |= (1<<PA0)|(1<<PA1);

        //counters for morse code
        uint16_t press_duration = 0; //time measured in centiseconds
        uint16_t release_duration = 0; //time measured in centiseconds

	//buffers for current letter and total message, and length counter for incrementing variable length message
	char *message = calloc(1,sizeof(char));
	char *current_letter = calloc(1,sizeof(char));
	int msg_index = 0;
	int letter_index = 0;
	int dot_or_dash = -1;

        while(1)
        {
                //read buttons
                uint8_t morse_b;//PINA & (1<<PA0);
		printf("1 for button is pressed, 0 for button is unpressed, 2 to check message\n");
		scanf("%d", &morse_b);
                uint8_t send_b = 1;//PINA & (1<<PA1);

		//for testing
		if(morse_b == 2)
		{
			send_b = 0;
		}
                if (send_b == 0) //checking for if send button is pressed
                {
                        //send message to other device
			printf("%s\n", message);
                        //reset
                        press_duration = 0;
                        release_duration = 0;
			reset_string(&message, &msg_index);
			reset_string(&current_letter, &letter_index);
			break;
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
				printf("new word\n");
				append_to_string(decode_letter(current_letter), &message, &msg_index);
				append_to_string('_', &message, &msg_index);
				reset_string(&current_letter, &letter_index);
                        }
                        else if (((3 * DOT) - LENIENCE < release_duration) && (release_duration < (3 * DOT) + LENIENCE)) //if delay is around 3s, that's the end of the letter. 250ms leniency
                        {
                                //append letter to message
				printf("new letter\n");
				printf("%s %d\n", message, msg_index);
				append_to_string(decode_letter(current_letter), &message, &msg_index);
				printf("%s %d\n", message, msg_index);
				reset_string(&current_letter, &letter_index);
                        }
			//updating values
			release_duration = 0;
                }
                else //if morse button is not pressed
                {
			//TESTING CODE
			printf("enter duration of button being pressed\n");
			scanf("%d", &press_duration);

                        //if we are in this code block, then morse_b is unpressed, and we need to count the length of release
                        release_duration++;
			printf("%s %d\n", current_letter, letter_index);
                        //checking for dot or dash
                        if ((DOT - LENIENCE < press_duration) && (press_duration < DOT + LENIENCE))//check for dot, 250ms of leniency
                        {
                                dot_or_dash = 0;
                        }
                        else if ((DASH - LENIENCE < press_duration) && (press_duration < DASH + LENIENCE))//check for dash, 250ms of leniency
                        {
                                dot_or_dash = 1;
                        }
                        else
                        {
                                continue;
                        }

                        //updates array for current letter from last button press
                        if (*(current_letter + LETTER_LENGTH - 1) == 0)//ensuring no oob indexing
                        {
                                if (dot_or_dash == 0)
                                {
                                        append_to_string('.', &current_letter, &letter_index);
                                }
                                else if (dot_or_dash == 1)
                                {
                                        append_to_string('-', &current_letter, &letter_index);
                                }
                        }
			printf("%s %d\n", current_letter, letter_index);
			printf("msg idx %d\n", msg_index);
                        //resetting necessary data
			dot_or_dash = -1;
                        press_duration = 0;
                }
                //due to this delay, the duration counter should be equivalent to number of ms/10
                //_delay_ms(DELAY);//debouncing delay
		sleep(1);

        }
}
