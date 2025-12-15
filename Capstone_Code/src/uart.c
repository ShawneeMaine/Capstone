#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "capstone_morse.h"

#define BIT_TIME (F_CPU / SOFTUART_BAUD)  // Timer1 ticks per bit
#define RX_SAMPLE_OFFSET (BIT_TIME / 2)  // sample in middle of bit

// --- TX state ---
static volatile uint8_t tx_byte;
static volatile uint8_t tx_mask;
static volatile bool tx_active = false;

// --- RX state ---
static volatile uint8_t rx_byte;
static volatile uint8_t rx_mask;
static volatile uint8_t rx_bits_received;
static volatile bool rx_active = false;
static volatile bool rx_ready = false;

// --- TX init ---
static void tx_init(void)
{
    SOFTUART_TX_DDR |= (1 << SOFTUART_TX_PIN);   // output
    SOFTUART_TX_PORT |= (1 << SOFTUART_TX_PIN);  // idle high

    // Timer1 CTC mode
    TCCR1B = (1 << WGM12);
    OCR1A = BIT_TIME;
    TIMSK1 |= (1 << OCIE1A); // enable compare
}

// --- RX init ---
static void rx_init(void)
{
	DDRB &= ~(1 << PB1);      // input
	PORTB |= (1 << PB1);      // enable pull-up

    // Pin change interrupt enable for PB1 (PCINT1)
    GIMSK |= (1 << WGM12);      // enable PCINT0..7
    PCMSK1 |= (1 << PCINT1);    // enable PCINT1
}

// --- public init ---
void softuart_init(void)
{
    tx_init();
    rx_init();
}

// --- TX ---
bool softuart_tx_busy(void) { return tx_active; }

void softuart_tx_byte(uint8_t b)
{
    while (tx_active); // wait if previous byte still sending

    tx_byte = b;
    tx_mask = 0x01;
    tx_active = true;

    // Send start bit immediately
    SOFTUART_TX_PORT &= ~(1 << SOFTUART_TX_PIN);
    TCNT1 = 0;
    TCCR1B |= (1 << CS10);  // start Timer1
}

void softuart_tx_bytes(const uint8_t *data, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        softuart_tx_byte(data[i]);
        while (tx_active); // wait for byte finish
    }
}

// --- RX buffer ---
static volatile uint8_t rx_buffer[SOFTUART_RX_BUFFER];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;

// --- RX ISR modified ---
ISR(TIM1_COMPA_vect)
{
    // TX handling (unchanged)
    if (tx_active)
    {
        if (tx_mask)
        {
            if (tx_byte & tx_mask)
                SOFTUART_TX_PORT |= (1 << SOFTUART_TX_PIN);
            else
                SOFTUART_TX_PORT &= ~(1 << SOFTUART_TX_PIN);
            tx_mask <<= 1;
        }
        else
        {
            SOFTUART_TX_PORT |= (1 << SOFTUART_TX_PIN);
            tx_active = false;
            TCCR1B &= ~(1 << CS10);
        }
    }

    // RX handling
    if (rx_active)
    {
        if (rx_bits_received < 8)
        {
            rx_byte >>= 1;
            if (SOFTUART_RX_PIN_REG & (1 << SOFTUART_RX_PIN))
                rx_byte |= 0x80;
            rx_bits_received++;
        }
        else
        {
            // store byte in buffer
            uint8_t next = (rx_head + 1) % SOFTUART_RX_BUFFER;
            if (next != rx_tail) // avoid overflow
            {
                rx_buffer[rx_head] = rx_byte;
                rx_head = next;
            }
            // stop receiving
            rx_active = false;
        }
    }
}

// --- RX read ---
bool softuart_rx_available(void)
{
    return rx_head != rx_tail;
}

uint8_t softuart_rx_read(void)
{
    if (rx_head == rx_tail) return 0;
    uint8_t val = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % SOFTUART_RX_BUFFER;
    return val;
}

#include <string.h>

void transmit_test(void) {
	//Transmit test
	strcpy(message, "A");
	softuart_tx_bytes((uint8_t*)message, 1);
	while(1) {
		if (softuart_rx_available())
			{
				uint8_t c = softuart_rx_read();
				message[0] = c;
				message[1] = '\0';
				update_display();				
			}
	}
}
