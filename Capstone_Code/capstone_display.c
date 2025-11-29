#include <stdio.h>

#include "capstone_morse.h"

void update_display()
{
	printf("Letter: ");
	printf(current_letter);
	printf("\n");

	printf("Message: ");
	printf(message);
	printf("\n");
}
