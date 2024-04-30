/*============================================================================
* To enable i2c
*
* 1. Open "nrf_drv_config.h"
* #define TWI0_ENABLED 1
*
* 2. Add header search path
*  \components_pxi\drivers_nrf\twi_master
*  \components_pxi\libraries\twi
*
* 3. Add bsp source files
*  \components_pxi\drivers_nrf\twi_master\nrf_drv_twi.c
*  \components_pxi\libraries\twi\app_twi.c
*
============================================================================*/

#include "pxi_nrf_i2c.h"

#include <string.h>

#include <nrf_gpio.h>

#include <app_util_platform.h>


#define MAX_PENDING_TRANSACTIONS    5
    

////============================================================================
//// Private global variables
////============================================================================
////static app_twi_t m_app_twi = APP_TWI_INSTANCE(0);
//static bool m_has_init = false;
//static pxi_nrf_i2c		i2c;

//============================================================================
// Private functions
//============================================================================
static nrf_twi_frequency_t to_nrf_twi_frequency_t(pxi_nrf_i2c_frequency frequency)
{
    nrf_twi_frequency_t twi_frequency_t;
    
    switch (frequency)
    {
        case PXI_NRF_I2C_FREQUENCY_100K:
            twi_frequency_t = NRF_TWI_FREQ_100K;
            break;
        
        case PXI_NRF_I2C_FREQUENCY_250K:
            twi_frequency_t = NRF_TWI_FREQ_250K;
            break;
        
        default:
        case PXI_NRF_I2C_FREQUENCY_400K:
            twi_frequency_t = NRF_TWI_FREQ_400K;
            break;
    }
    
    return twi_frequency_t;
}

static nrf_gpio_pin_pull_t to_nrf_gpio_pin_pull_t(pxi_nrf_i2c_pin_pull pin_pull)
{
    nrf_gpio_pin_pull_t gpio_pin_pull;
    
    switch (pin_pull)
    {
        case PXI_NRF_I2C_PIN_PULLDOWN:
            gpio_pin_pull = NRF_GPIO_PIN_PULLDOWN;
            break;
        
        case PXI_NRF_I2C_PIN_PULLUP:
            gpio_pin_pull = NRF_GPIO_PIN_PULLUP;
            break;
        
        default:
            gpio_pin_pull = NRF_GPIO_PIN_NOPULL;
            break;
    }
    
    return gpio_pin_pull;
}


//============================================================================
// Public functions
//============================================================================
void pxi_nrf_i2c_get_default_setting(pxi_nrf_i2c_setting *i2c_setting)
{
    if (!i2c_setting)
        return;
    
    memset(i2c_setting, 0, sizeof(*i2c_setting));
    i2c_setting->frequency = PXI_NRF_I2C_FREQUENCY_400K;
    i2c_setting->pin_pull = PXI_NRF_I2C_PIN_NOPULL;
}


