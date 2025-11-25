CC = avr-gcc
MCU = attiny84
F_CPU = 8000000UL
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os
OBJCOPY = avr-objcopy

#morse
TARG1 = capstone_morse
TARG1_SRC = capstone_morse.c
TARG1_OBJ = $(TARG1_SRC:.c=.o)

#display
TARG2 = capstone_display
TARG2_SRC = capstone_display.c
TARG2_OBJ = $(TARG2_SRC:.c=.o)

all: $(TARG1).hex $(TARG2).hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARG1).elf: $(TARG1_OBJ)
	$(CC) $(CFLAGS) $^ -o $@
$(TARG2).elf: $(TARG2_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(TARG1).hex: $(TARG1).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
$(TARG2).hex: $(TARG2).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

clean:
	rm -f *.o *.elf *.hex
