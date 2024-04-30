

#ifndef __APP_I2C_H__
#define __APP_I2C_H__

#include <stdint.h>

void app_twi_init (void);

void app_twi_uninit(void);

void stk832x_register_set(uint8_t reg_id, uint8_t val);
void stk832x_register_get(uint8_t reg_id, uint8_t *val);
void stk832x_registers_get(uint8_t reg_id, uint8_t *val, uint8_t size);

signed char stk832x_registers_set(unsigned char regAddr, unsigned char *regValue, unsigned char count);

#endif
