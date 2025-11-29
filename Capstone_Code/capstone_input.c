/*
Benjamin Mock
Created 28 November 2025
This code is to take inputs from two push buttons connected to an ATTINY84.
It uses the ATTINY84's internal pull-up resistors, interrupts, and software debouncing.
Button 1 is connected to pin PA2 on the ATTINY. This is the SEND MESSAGE button for this project.
Button 2 is connected to pin PA3 on the ATTINY. This is the MORSE CODE INPUT button for this project.
*/


#include "capstone_input.h"

volatile uint32_t millis = 0;

//Raw states from ISR
volatile uint8_t raw_b1 = 1;
volatile uint8_t raw_b2 = 1;

//Debouncing flag
volatile uint8_t deb_flag = 0;

//Timestamp used for debounce filtering
volatile uint32_t last_change_b1 = 0;
volatile uint32_t last_change_b2 = 0;

//For measuring duration of Button 2
uint32_t b2_prev_timestamp = 0;
uint32_t b2_press_time = 0;//in ms


//Tracker of what button is currently pressed(0=None)
uint8_t button_pressed=0;


//Timer0: 1ms interrupt
ISR(TIM0_COMPA_vect) {
    millis++;
}


//PCINT interrupt for PA2 & PA3
ISR(PCINT0_vect) {
    //PINA – Port A Input Pins
    uint8_t pins = PINA;
    uint32_t now = millis;
    deb_flag = 1;

    //Button 1
    uint8_t current_b1 = (pins & (1 << BUTTON1_PIN)) ? 1 : 0;
    if (current_b1 != raw_b1) {
        raw_b1 = current_b1;
        last_change_b1 = now;   //Debounce timer
    }

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


//Also for last_change_b1 and b2
uint32_t get_b1_timestamp() {
    uint32_t b1;
    cli();  //Disable interrupts
    b1 = last_change_b1;
    sei();  //Turn interrupts back on
    return b1;
}


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
    TCCR0B = (1 << CS01);   //Prescale 8
    OCR0A = 124;    //1ms at 1MHz
    TIMSK0 = (1 << OCIE0A); //Enable interrupts
}


//Initialize GPIO and pin-change interrupts
void io_init() {
    //Enable pullups
    PORTA |= (1 << BUTTON1_PIN) | (1 << BUTTON2_PIN);

    //Enable pin change interrupts on PA2 & PA3 (PCINT2 & PCINT3)
    GIMSK |= (1 << PCIE0);  //enable PCINT[7:0]
    PCMSK0 |= (1 << PCINT2) | (1 << PCINT3);    //unmask pins
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
    uint32_t b1_current_timestamp = get_b1_timestamp();
    uint32_t b2_current_timestamp = get_b2_timestamp();


    if(deb_flag) {//Button press detected
        if(b1_current_timestamp > b2_current_timestamp) {   //Debound button 1
            if ((now - b1_current_timestamp) >= DEBOUNCE_MS) {
                deb_flag = 0;
                button_pressed = (button_pressed == 0) ? 1 : 0;
            }
        }
        else {  //Debound button 2
            if ((now - b2_current_timestamp) >= DEBOUNCE_MS) {
                deb_flag = 0;
                b2_press_time = b2_current_timestamp-b2_prev_timestamp;
                button_pressed = (button_pressed == 0) ? 2 : 0;
            }
        }
    }


}