#pragma once


// bsp
#include "nrf_drv_twi.h"


//============================================================================
// Definitions
//============================================================================
#define PXI_NRF_I2C_MAX_COMM_SIZE  (254)
#define SLAVE_ID	(0x40)

typedef enum
{
    PXI_NRF_I2C_FREQUENCY_100K = 100,
    PXI_NRF_I2C_FREQUENCY_250K = 250,
    PXI_NRF_I2C_FREQUENCY_400K = 400,
    
} pxi_nrf_i2c_frequency;

typedef enum
{
    PXI_NRF_I2C_PIN_NOPULL,
    PXI_NRF_I2C_PIN_PULLDOWN,
    PXI_NRF_I2C_PIN_PULLUP,
    
} pxi_nrf_i2c_pin_pull;


typedef struct
{
    pxi_nrf_i2c_frequency   frequency;
    pxi_nrf_i2c_pin_pull    pin_pull;
    
} pxi_nrf_i2c_setting;


typedef struct
{
    uint16_t    slave_addr;
    
} pxi_nrf_i2c_config;


typedef struct
{
    pxi_nrf_i2c_config  config;
    
} pxi_nrf_i2c;


//============================================================================
// Public functions
//============================================================================
void                pxi_nrf_i2c_set_clock_speed(nrf_twi_frequency_t hz);
nrf_twi_frequency_t pxi_nrf_i2c_get_clock_speed(void);

void        pxi_nrf_i2c_get_default_setting(pxi_nrf_i2c_setting *i2c_setting);
ret_code_t  pxi_nrf_i2c_init(void);
void        pxi_nrf_i2c_deinit(void);


ret_code_t  pxi_nrf_i2c_write(uint8_t addr, const uint8_t *data, size_t write_size);
ret_code_t  pxi_nrf_i2c_read(uint8_t addr, uint8_t *data, size_t read_size);

