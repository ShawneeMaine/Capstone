#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TX_PORT PORTB
#define TX_DDR  DDRB
#define TX_PIN  PB0

#define RX_PORT PORTB
#define RX_DDR  DDRB
#define RX_PINR PINB
#define RX_PIN  PB1

#define FREQ 1000000UL
#define BAUD 2400

#define TIM_BITS (1<<CS00)
#define BIT_TIME (1000000UL/BAUD)
#define OCR0A_VAL (BIT_TIME - 1)

extern volatile uint8_t rx_byte;
extern volatile uint8_t rx_bit_index;
extern volatile uint8_t rx_busy;
extern volatile uint8_t rx_done;

void uart_init(void);
void tx_uart(uint8_t data);

#endif
