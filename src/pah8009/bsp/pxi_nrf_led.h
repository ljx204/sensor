#pragma once


// c
#include <stdint.h>
#include <stdbool.h>


//============================================================================
// Definitions
//============================================================================
typedef enum
{
    PXI_NRF_LED_1,
    PXI_NRF_LED_2,
    PXI_NRF_LED_3,
    PXI_NRF_LED_4,
} pxi_nrf_led;


//============================================================================
// Public functions
//============================================================================
bool    pxi_nrf_led_init(void);

bool    pxi_nrf_led_enable(pxi_nrf_led led, bool enable);
void    pxi_nrf_led_toggle(pxi_nrf_led led);

