//attiny headers
#include <avr/io.h>
#include <util/delay.h>
//to account for 10ms delay, each value is its actual time in ms/10
#DEFINE DOT = 100;
#DEFINE DASH = 300;

//defining buffer for storing the current message to be sent
char *message = NULL;
size_t len = 0;
//function to append letter to buffer
void append_to_message(char c)
{
	message = realloc(buf, len+1);
	message(len) = c;
	len++
}
int main
{
		//set pins 10 and 11 to be input. high when button not pressed, low when button is pressed
		DDRA &= ~(1<<PA0)|~(1<<PA1);
		//enable internal pullup resistors
		PORTA |= (1<<PA0)|(1<<PA1);
		//counters for morse code
		uint8_t press_duration = 0;
		uint8_t release_dration = 0;
		int current_letter[] = [-1,-1,-1,-1];
		int index = 0;
		while(1)
		{
			//read buttons
			uint8_t morse_b = PINA & (1<<PA0);
			uint8_t send_b = PINA & (1<<PA1);
			int dot_or_dash = -1;

			if (morse_b == 0)//if morse button is pressed
			{
				//count how long button is being pressed for
				press_duration++;
				if (release_duration >= 7 * DOT)//new word
				{
					//append last character, append space character, reset current letter array
					current_letter[] = [-1,-1,-1,-1];
					release_duration = 0;
				}
				else if (DOT - 25 < release_duration < DOT + 25) //if delay is around 3s, that's the end of the letter. 250ms leniency
				{
					//append letter to message, reset current letter array
					current_letter[] = [-1,-1,-1,-1];
					release_duration = 0;
				}
			}
			else //if morse button is not pressed
			{
				//if we are in this code block, then morse_b is unpressed, and we need to count the length of release
				release_duration++;
				
				//checking for dot or dash
				if (DOT - 25 < press_duration < DOT + 25)//check for dot, 250ms of leniency
				{
					dot_or_dash = 0;
				}
				else if (DASH - 25 < press_duration < DASH + 25)//check for dash, 250ms of leniency
				{
					dot_or_dash = 1;
				}
				else
				{
					continue;
				}
				
				//handling morse word
				if (dot_or_dash == 0)
				{
					current_letter[index] = 0;
					index++;
				}
				else if (dot_or_dash == 1);
				{
					current_letter[index] = 1;
					index++;
				}
				
				//resetting necessary data
				press_duration = 0;
				dot_or_dash = -1;
			}
			//due to this delay, the duration counter should be equivalent to number of ms/10
			_delay_ms(10);//debouncing delay
			
		}
}