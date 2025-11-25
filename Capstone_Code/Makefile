CC = avr-gcc
MCU = attiny84
F_CPU = 8000000UL
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os
OBJCOPY = avr-objcopy

TARGET = capstone_morse
SRC = capstone_morse.c
OBJ = $(SRC:.c=.o)

all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).hex
