#include <avr/io.h>
#include <util/delay.h>
#include "usi_i2c.h"

#define OLED_ADDR 0x3C   //given I2C address of the display

#define SDA PA6
#define SCL PA4

#define SDA_HIGH() (PORTA |= (1<<SDA))
#define SDA_LOW()  (PORTA &= ~(1<<SDA))

#define SCL_HIGH() (PORTA |= (1<<SCL))
#define SCL_LOW()  (PORTA &= ~(1<<SCL))

void i2c_init(void) {
    DDRA |= (1<<SCL) | (1<<SDA);    // outputs
    SDA_HIGH();
    SCL_HIGH();
}

static void i2c_transfer(uint8_t data) {
    USIDR = data;
    USISR = (1<<USIOIF);
    do {
        _delay_us(5);
        USICR = (1<<USIWM1)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
    } while(!(USISR & (1<<USIOIF)));
}

uint8_t i2c_start(uint8_t address) {
    SDA_LOW(); _delay_us(5);
    SCL_LOW();
    return i2c_write(address);
}

uint8_t i2c_write(uint8_t data) {
    i2c_transfer(data);
    // receive ACK bit
    USIDR = 0xFF;
    i2c_transfer(0x00);
    return (USIDR & 0x01);
}

uint8_t i2c_read_ack(void) {
    USIDR = 0xFF;
    i2c_transfer(0x00);
    return USIDR;
}

uint8_t i2c_read_nack(void) {
    USIDR = 0xFF;
    i2c_transfer(0x80);
    return USIDR;
}

void i2c_stop(void) {
    SDA_LOW();
    _delay_us(5);
    SCL_HIGH();
    _delay_us(5);
    SDA_HIGH();
}

void oled_set_cursor(uint8_t page, uint8_t column) {
    i2c_start(OLED_ADDR << 1);
    i2c_write(0x00);               
    i2c_write(0xB0 | page);        
    i2c_write(0x00 | (column & 0x0F));
    i2c_write(0x10 | (column >> 4));
    i2c_stop();
}

// Write a 6-byte character to the OLED
void oled_write_char6(const uint8_t data[6]) {

    i2c_start(OLED_ADDR << 1);

    i2c_write(0x40);              

    for (uint8_t i = 0; i < 6; i++) {
        i2c_write(data[i]);
    }

    i2c_stop();
}

void oled_init() {
    i2c_start(OLED_ADDR << 1);
    i2c_write(0x00);  // control byte = command

    //Initialization commands
    i2c_write(0xAE); //Display OFF
    i2c_write(0xD5); //Set display clock divide ratio
    i2c_write(0x80); //Recommended value
    i2c_write(0xA8); //Set multiplex
    i2c_write(0x3F); //64MUX
    i2c_write(0xD3); //Set display offset
    i2c_write(0x00); //No offset
    i2c_write(0x40); //Set start line to 0
    i2c_write(0x8D); //Charge pump
    i2c_write(0x14); //Enable charge pump
    i2c_write(0x20); //Memory addressing mode
    i2c_write(0x00); //Horizontal addressing mode
    i2c_write(0xA1); //Segment remap
    i2c_write(0xC8); //COM scan direction
    i2c_write(0xDA); //COM pins hardware config
    i2c_write(0x12);
    i2c_write(0x81); //Contrast
    i2c_write(0xCF);
    i2c_write(0xD9); //Pre-charge
    i2c_write(0xF1);
    i2c_write(0xDB); //VCOM detect
    i2c_write(0x40);
    i2c_write(0xA4); //Entire display ON
    i2c_write(0xA6); //Normal display
    i2c_write(0xAF); //Display ON

    i2c_stop();
}