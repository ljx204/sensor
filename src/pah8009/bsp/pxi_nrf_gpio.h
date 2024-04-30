#pragma once


// c
#include <stdint.h>
#include <stdbool.h>


//============================================================================
// Definitions
//============================================================================
typedef enum
{
    PXI_NRF_GPIO_PULL_TYPE_NOPULL,
    PXI_NRF_GPIO_PULL_TYPE_PULLDOWN,
    PXI_NRF_GPIO_PULL_TYPE_PULLUP,
} pxi_nrf_gpio_pull_type;


// slave -> board
typedef enum
{
    PXI_NRF_GPIO_IN_6 = 6,
    PXI_NRF_GPIO_IN_14 = 14,
} pxi_nrf_gpio_in;


// board -> slave
typedef enum
{
    PXI_NRF_GPIO_OUT_22 = 22,
    PXI_NRF_GPIO_OUT_23 = 23,
    PXI_NRF_GPIO_OUT_24 = 24,
    PXI_NRF_GPIO_OUT_25 = 25,
    //PXI_NRF_GPIO_OUT_26 = 26,    // Had been used by I2C INSTANCE 0
    //PXI_NRF_GPIO_OUT_27 = 27,    // Had been used by I2C INSTANCE 0
    PXI_NRF_GPIO_OUT_28 = 28,
    PXI_NRF_GPIO_OUT_29 = 29,
    PXI_NRF_GPIO_OUT_30 = 30,
    PXI_NRF_GPIO_OUT_31 = 31,
    
} pxi_nrf_gpio_out;


typedef enum {
    PXI_NRF_GPIO_STATUS_LOW,
    PXI_NRF_GPIO_STATUS_HIGH,
} pxi_nrf_gpio_status;


typedef void (*pxi_nrf_gpio_interrupt_handle)(pxi_nrf_gpio_in gpio_in, pxi_nrf_gpio_status gpio_status);


//============================================================================
// Public functions
//============================================================================
// slave -> board
bool    pxi_nrf_gpio_in_init(void);
void    pxi_nrf_gpio_in_set_interrupt_handler(pxi_nrf_gpio_interrupt_handle handler);
void    pxi_nrf_gpio_in_pull(pxi_nrf_gpio_in gpio_in, pxi_nrf_gpio_pull_type pull_type);
bool    pxi_nrf_gpio_in_is_high(pxi_nrf_gpio_in gpio_in);


// board -> slave
bool    pxi_nrf_gpio_out_enable(pxi_nrf_gpio_out gpio_out);

void    pxi_nrf_gpio_out_toggle(pxi_nrf_gpio_out gpio_out);
void    pxi_nrf_gpio_out_write(pxi_nrf_gpio_out gpio_out, bool high);
bool    pxi_nrf_gpio_out_is_high(pxi_nrf_gpio_out gpio_out);


