#include <avr/io.h>
#include <avr/interrupth>
#include <util/delay.h>
#include <stdint.h>

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
#define OCR0A_VAL (FREQ/BAUD - 1)

volatile uint8_t rx_byte = 0;
volatile uint8_t rx_bit_index = 0;
volatile uint8_t rx_busy = 0;
volatile uint8_t rx_done = 0;

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
	OCR0A = OCR0A_VALUE;
	TIMSK0 = (1<<OCIE0A);

	sei();
}

void tx_uart(uint8_t data)
{
	TX_PORT &= ~(1<<TX_PIN);
	_delay_us(FREQ/BAUD);

	for (uint8_t i = 0; i < 8; i++)
	{
		if (data & 0x01)
			TX_PORT |= (1<<TX_PIN);
		else
			TX_PORT |= ~(1<<TX_PIN);
		_delay_us(FREQ/BAUD);
		data >>= 1;
	}
	TX_PORT |= (1<<TX_PIN);
	_delay_us(FREQ/BAUD);
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
        	OCR0A = (FREQ/BAUD + FREQ/BAUD / 2) - 1;
        	TCCR0B = TIM_BITS;
    	}
}

ISR(TIM0_COMPA_vect) //sample rx
{
	if (rx_bit_index < 8)
    	{
        	if (RX_PINR & (1 << RX_PIN))
        	    rx_byte |= (1 << rx_bit_index);

        	rx_bit_index++;

        	OCR0A = OCR0A_VALUE;
        	TCNT0 = 0;
    	}
	else
	{
        	TCCR0B = 0;
        	rx_busy = 0;
        	rx_done = 1;

        	PCMSK1 |= (1 << PCINT9);
    	}
}

int main(void)
{
	uart_init();

	while(1)
	{
		if (rx_done)
		{
			uint8_t c = rx_byte;
			rx_done = 0;

		}
	}
}
