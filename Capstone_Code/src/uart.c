#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t rx_byte = 0;
volatile uint8_t rx_bit_index = 0;
volatile uint8_t rx_busy = 0;
volatile uint8_t rs_done = 0;

void uart_init(void)
{
	//idle high
	TX_DDR |= (1<<TX_PIN);
	TX_PORT |= (1<<TX_PIN);

	//rx pullup
	RX_DDR &= ~(1<<RX_PIN);
	RX_PORT |= (1<<RX_PIN);

	//enable interrupt
	GIMSK |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT9);

	//timer setup
	TCCR0A = (1<<WGM01);
	TCCR0B = 0;
	OCR0A = OCR0A_VAL;
	TIMSK0 = (1<<OCIE0A);

	sei();
}

void tx_uart(uint8_t data)
{
	TX_PORT &= ~(1<<TX_PIN);
	_delay_us(BIT_TIME);

	for (uint8_t i = 0; i < 8; i++)
	{
		if (data & 0x01)
			TX_PORT |= (1<<TX_PIN);
		else
			TX_PORT |= ~(1<<TX_PIN);
		_delay_us(BIT_TIME);
		data >>= 1;
	}
	TX_PORT |= (1<<TX_PIN);
	_delay_us(BIT_TIME);
}
ISR(PCINT1_vect) //start rx
{
	if (!rx_busy && !(RX_PINR & (1 << RX_PIN)))
	{
        	rx_busy = 1;
        	rx_done = 0;
        	rx_bit_index = 0;
        	rx_byte = 0;

        	PCMSK1 &= ~(1 << PCINT9);

        	TCNT0 = 0;
        	OCR0A = (BIT_TIME + BIT_TIME / 2) - 1;
        	TCCR0B = TIM_BITS;
    	}
}
