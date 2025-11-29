#config
CC      = avr-gcc
MCU     = attiny84
F_CPU = 1000000UL

CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os
OBJCOPY = avr-objcopy
AVRSIZE = avr-size

TARGET  = capstone

SRC = \
    capstone_main.c \
    capstone_display.c \
    capstone_morse.c \
    capstone_input.c

OBJ = $(SRC:.c=.o)

#build rules
all: $(TARGET).hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
	$(AVRSIZE) --mcu=$(MCU) --format=avr $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

clean:
	rm -f *.o *.elf *.hex
