AVR_CC = avr-gcc
MCU = attiny84
F_CPU = 1000000UL
AVR_CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os
OBJCOPY = avr-objcopy
AVRSIZE = avr-size

#firmware source files
AVR_SRCS = capstone_main.c capstone_display.c capstone_morse.c capstone_input.c
AVR_OBJS = $(AVR_SRCS:.c=.o)
AVR_HEX = capstone.hex
AVR_ELF = capstone.elf

#PC test
HOST_CC = gcc
HOST_CFLAGS = -Wall -O2 -DHOST
HOST_SRCS = test.c capstone_display.c capstone_input.c capstone_morse.c
HOST_OBJS = $(HOST_SRCS:.c=.o)
HOST_EXE = test.exe

#default target
all: $(AVR_HEX)

#AVR firmware build
%.o: %.c
	$(AVR_CC) $(AVR_CFLAGS) -c $< -o $@

$(AVR_ELF): $(AVR_OBJS)
	$(AVR_CC) $(AVR_CFLAGS) $^ -o $@
	$(AVRSIZE) --target=elf32-avr $@

$(AVR_HEX): $(AVR_ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

clean:
	rm -f *.o *.elf *.hex

#host test harness build
HOST_SRCS = test.c capstone_display.c input_simulation.c capstone_morse.c
HOST_OBJS = $(HOST_SRCS:.c=.o)
HOST_EXE = test.exe

host: $(HOST_EXE)

%.o: %.c
	$(HOST_CC) $(HOST_CFLAGS) -c $< -o $@

$(HOST_EXE): $(HOST_OBJS)
	$(HOST_CC) $(HOST_CFLAGS) $^ -o $@

clean-host:
	rm -f $(HOST_OBJS) $(HOST_EXE)

.PHONY: all clean host clean-host flash
