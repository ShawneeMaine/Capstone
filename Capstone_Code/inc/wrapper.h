#ifndef I2C_WRAPPER_H
#define I2C_WRAPPER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void twm_init(void);
uint8_t twm_begin_transmission(uint8_t address);
uint8_t twm_write(uint8_t data);
uint8_t twm_end_transmission(void);

uint8_t twm_request_from(uint8_t address, uint8_t length);
uint8_t twm_available(void);
uint8_t twm_read(void);

#ifdef __cplusplus
}
#endif

#endif
