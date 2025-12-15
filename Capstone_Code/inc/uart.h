#ifndef SOFTUART_H
#define SOFTUART_H

#include <stdint.h>
#include <stdbool.h>

#define SOFTUART_TX_PIN 0  // PB0
#define SOFTUART_TX_PORT PORTB
#define SOFTUART_TX_DDR  DDRB

#define SOFTUART_RX_PIN PB1
#define SOFTUART_RX_PIN_REG PINB   // PINB reads the logic state of PORTB

#define SOFTUART_RX_BUFFER 21

bool softuart_rx_available(void);
uint8_t softuart_rx_read(void);

#define SOFTUART_BAUD 9600UL
#define F_CPU 1000000UL

void softuart_init(void);
bool softuart_tx_busy(void);
void softuart_tx_byte(uint8_t b);
void softuart_tx_bytes(const uint8_t *data, uint8_t len);

// Optional: call in main loop to process completed TX if needed
void softuart_update(void);
void transmit_test(void);

#endif
