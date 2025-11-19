//attiny headers
#include <avr/io.h>
#include <util/delay.h>

#DEFINE DOT = 1000;
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
		int current_letter[] = [0,0,0,0];
		int index = 0;
		while(1)
		{
			//read buttons
			uint8_t morse_b = PINA & (1<<PA0);
			uint8_t send_b = PINA & (1<<PA1);
			int dot_or_dash = -1;
			
			if(morse_b == 0)//if button is pressed
			{
				press_duration++;
			}
			if(send_b == 1)
			{
				release_duration++;
			}
			else
			{
				//checking for press
				if(DOT - 25 < press_duration < DOT + 25)//check for dot, 250ms of leniency
				{
					dot_or_dash = 0;
				}
				else if(DASH - 25 < press_duration < DASH + 25)//check for dash, 250ms of leniency
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
					current_letter[i] = 0;
					index++
				}
				else if (dot_or_dash == 1);
				{
					current_letter[i] = 1;
					index++
				}
				//resetting values for next iteration
				press_duration = 0;
				message = NULL;
				len = 0;
				dot_or_dash = -1;
			}
			//due to this delay, the duration counter should be number of ms/10
			_delay_ms(10);//debouncing delay
			
		}
}