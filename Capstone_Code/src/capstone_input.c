/*
Benjamin Mock
Created 28 November 2025
This code is to take inputs from two push buttons connected to an ATTINY84.
It uses the ATTINY84's internal pull-up resistors, interrupts, and software debouncing.
Button 1 is connected to pin PA2 on the ATTINY. This is the SEND MESSAGE button for this project.
Button 2 is connected to pin PA3 on the ATTINY. This is the MORSE CODE INPUT button for this project.
*/
#include "uart.h"

#include "capstone_input.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t millis = 0;

//Raw states from ISR
volatile uint8_t raw_b2 = 1;

//Debouncing flag
volatile uint8_t deb_flag = 0;

//Timestamp used for debounce filtering
volatile uint32_t last_change_b2 = 0;

//For measuring duration of Button 2
uint32_t b2_prev_timestamp = 0;
uint32_t b2_press_time = 0;//in ms

//stable state trackers
uint8_t deb_b2 = 1;

//Tracker of what button is currently pressed(0=None)
uint8_t button_pressed=0;


//Timer0: 1ms interrupt
//Also handling the RX (receiving) interrupts for receiving data from other device
ISR(TIM0_COMPA_vect) {
    millis++;
    if (rx_bit_index < 8)
    {
        if (RX_PINR & (1 << RX_PIN))
            rx_byte |= (1 << rx_bit_index);

        rx_bit_index++;

        OCR0A = OCR0A_VAL;
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


//PCINT interrupt for PA2 & PA3
ISR(PCINT0_vect) {
    //PINA – Port A Input Pins
    uint8_t pins = PINA;
    uint32_t now = millis;
    deb_flag = 1;

    //Button 2
    uint8_t current_b2 = (pins & (1 << BUTTON2_PIN)) ? 1 : 0;
    if (current_b2 != raw_b2) {
        raw_b2 = current_b2;
        last_change_b2 = now;   //Debounce timer
    }
}


//Disable interrupts to read millis
uint32_t millis_now() {
    uint32_t m;
    cli();  //Disable interrupts
    m = millis;
    sei();  //Turn interrupts back on
    return m;
}


//Also for last_change_b2
uint32_t get_b2_timestamp() {
    uint32_t b2;
    cli();  //Disable interrupts
    b2 = last_change_b2;
    sei();  //Turn interrupts back on
    return b2;
}

//Initialize Timer0 for 1ms intervals
void timer0_init() {
    TCCR0A |= (1 << WGM01); //Clear Timer on Compare (CTC) mode
    TCCR0B |= (1 << CS01);   //Prescale 8
    OCR0A = 124;    //1ms at 1MHz
    TIMSK0 = (1 << OCIE0A); //Enable interrupts
}


//Initialize GPIO and pin-change interrupts
void io_init() {
    //Enable pullups
    PORTA |= (1 << BUTTON2_PIN);

    //Enable pin change interrupts on PA3 (PCINT3)
    GIMSK |= (1 << PCIE0);  //enable PCINT[7:0]
    PCMSK0 |= (1 << PCINT3);    //unmask pins
}


void master_button_init() {
    //Running all the inits
    io_init();
    timer0_init();
    sei();
}


//Button reading and software debounce
void read_buttons() {
    uint32_t now = millis_now();
    uint32_t b2_current_timestamp = get_b2_timestamp();

   if(deb_flag) {//Button press detected
            if ((now - b2_current_timestamp) >= DEBOUNCE_MS) {
		if (raw_b2 != deb_b2) {
		    deb_b2 = raw_b2;
		    if (deb_b2 == 0) {
			b2_prev_timestamp = b2_current_timestamp;
			button_pressed = 1;
		    }
		    else {
			b2_press_time = b2_current_timestamp - b2_prev_timestamp;
			b2_prev_timestamp = b2_current_timestamp;
			button_pressed = 0;
		    }
		    deb_flag = 0;
		}
            }
     }


}
