#include "pxi_nrf_led.h"

// bsp
#include <boards.h>
#include <nrf_gpio.h>


//============================================================================
// Private global variables
//============================================================================
static const uint32_t led_list[] = LEDS_LIST;


//============================================================================
// Public functions
//============================================================================
bool pxi_nrf_led_init(void)
{
    uint32_t i = 0;
    
    for (i = 0; i < LEDS_NUMBER; i++)
    {
        nrf_gpio_cfg_output(led_list[i]);
        nrf_gpio_pin_set(led_list[i]);
    }
    
    
    return true;
}

bool pxi_nrf_led_enable(pxi_nrf_led led, bool enable)
{
    if (enable)
        nrf_gpio_pin_clear(led_list[led]);
    else
        nrf_gpio_pin_set(led_list[led]);
    
    return true;
}

void pxi_nrf_led_toggle(pxi_nrf_led led)
{
    nrf_gpio_pin_toggle(led_list[led]);
}
