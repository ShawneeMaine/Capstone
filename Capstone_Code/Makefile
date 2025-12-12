#Toolchain
MCU      = attiny84
AVR_CC   = avr-gcc
AVR_CXX  = avr-g++ #Compiler for c++
F_CPU = 1000000UL
AVR_CFLAGS = -mmcu=$(MCU) -Wall -Os -DF_CPU=$(F_CPU)
AVR_CXXFLAGS = $(AVR_CFLAGS)
OBJCOPY  = avr-objcopy
AVRSIZE  = avr-size

HOST_CC  = gcc
HOST_CFLAGS = -Wall -O2

#Directories
SRC_DIR   = src
INC_DIR   = inc
TEST_DIR = test
BUILD_DIR = build

#Find sources
#AVR firmware: all .c and .cpp files except test files
AVR_SRCS = $(wildcard $(SRC_DIR)/*.c)
AVR_CPP_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
AVR_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(AVR_SRCS)) \
  $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.cpp.o,$(AVR_CPP_SRCS))
AVR_ELF  = $(BUILD_DIR)/capstone.elf
AVR_HEX  = $(BUILD_DIR)/capstone.hex


#Host test: all test/something.c files | exclude src/capstone_input.c, src/capstone_morse_test.c
HOST_SRCS = $(filter-out $(SRC_DIR)/capstone_main.c $(SRC_DIR)/capstone_input.c $(SRC_DIR)/capstone_morse.c, $(AVR_SRCS)) $(wildcard $(TEST_DIR)/*.c)
HOST_OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(HOST_SRCS:.c=.test.o)))
HOST_EXE  = $(BUILD_DIR)/test.exe

#Default target (AVR build)
all: $(AVR_HEX) avr-size-report

#AVR rules
#C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(AVR_CC) $(AVR_CFLAGS) -I$(INC_DIR) -c $< -o $@

#C++ files
$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(AVR_CXX) $(AVR_CXXFLAGS) -I$(INC_DIR) -c $< -o $@

$(AVR_ELF): $(AVR_OBJS)
	$(AVR_CXX) $(AVR_CXXFLAGS) $^ -o $@

$(AVR_HEX): $(AVR_ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

#Host test rules
.PHONY: test

test: $(HOST_EXE)

vpath %.c $(SRC_DIR) $(TEST_DIR)

$(BUILD_DIR)/%.test.o: %.c | $(BUILD_DIR)
	$(HOST_CC) $(HOST_CFLAGS) -I$(INC_DIR) -c $< -o $@

$(HOST_EXE): $(HOST_OBJS)
	$(HOST_CC) $(HOST_CFLAGS) $^ -o $@

#Make build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

#Size report for AVR build
avr-size-report: $(AVR_ELF)
	@echo "AVR Firmware Size Report"
	$(AVRSIZE) --target=elf32-avr $(AVR_ELF)
	@echo

clean:
	rm -rf $(BUILD_DIR)
