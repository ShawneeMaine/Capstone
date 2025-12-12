#include <TinyWireM.h>
#include "wrapper.h"

void twm_init(void) {
    TinyWireM.begin();
}

uint8_t twm_begin_transmission(uint8_t address) {
    TinyWireM.beginTransmission(address);
    return 0;   // TinyWireM doesn’t return anything here
}

uint8_t twm_write(uint8_t data) {
    return TinyWireM.write(data);  // returns 1 if OK
}

uint8_t twm_end_transmission(void) {
    return TinyWireM.endTransmission();
}

uint8_t twm_request_from(uint8_t address, uint8_t length) {
    return TinyWireM.requestFrom(address, length);
}

uint8_t twm_available(void) {
    return TinyWireM.available();
}

uint8_t twm_read(void) {
    return TinyWireM.read();
}
