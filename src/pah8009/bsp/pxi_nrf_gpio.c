#include "pxi_nrf_gpio.h"


// bsp
#include <nrf_gpio.h>
#include <app_gpiote.h>

// c
#include <stdlib.h>


//============================================================================
// Macro
//============================================================================
//#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof(arr[0]))
    

//============================================================================
// Definitions
//============================================================================
#define APP_GPIOTE_MAX_USERS            1


//============================================================================
// Private variables
//============================================================================
static const pxi_nrf_gpio_in pxi_nrf_gpio_in_list[] = {
    PXI_NRF_GPIO_IN_6,
   // PXI_NRF_GPIO_IN_14,
};

static app_gpiote_user_id_t   _gpiote_user_id;

static pxi_nrf_gpio_interrupt_handle _gpio_interrupt_handler = NULL;


//============================================================================
// Private functions
//============================================================================
// GPIOTE event handler.
void _gpiote_event_handler(uint32_t const * p_event_pins_low_to_high, uint32_t const *p_event_pins_high_to_low)
{
    if (!_gpio_interrupt_handler)
        return;
    
    if (*p_event_pins_low_to_high > 0)
    {
        for (int i = 0; i < ARRAY_SIZE(pxi_nrf_gpio_in_list); ++i)
        {
            if (IS_SET(*p_event_pins_low_to_high, pxi_nrf_gpio_in_list[i]))
                _gpio_interrupt_handler(pxi_nrf_gpio_in_list[i], PXI_NRF_GPIO_STATUS_HIGH);
        }
    }
    
    if (*p_event_pins_high_to_low > 0)
    {
        for (int i = 0; i < ARRAY_SIZE(pxi_nrf_gpio_in_list); ++i)
        {
            if (IS_SET(*p_event_pins_high_to_low, pxi_nrf_gpio_in_list[i]))
                _gpio_interrupt_handler(pxi_nrf_gpio_in_list[i], PXI_NRF_GPIO_STATUS_LOW);
        }
    }
}

//============================================================================
// Public functions
//============================================================================
bool pxi_nrf_gpio_in_init(void)
{
    uint32_t low_to_high_bitmask = 0, high_to_low_bitmask = 0;
    uint32_t ret = 0;
    
    APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);
    
    // low -> high
    for (int i = 0; i < ARRAY_SIZE(pxi_nrf_gpio_in_list); ++i)
    {
        SET_BIT(low_to_high_bitmask, pxi_nrf_gpio_in_list[i]);
    }
    
    // high -> low
	for (int i = 0; i < ARRAY_SIZE(pxi_nrf_gpio_in_list); ++i)
    {
        SET_BIT(high_to_low_bitmask, pxi_nrf_gpio_in_list[i]);
    }
    
    ret = app_gpiote_user_register(&_gpiote_user_id,
                                  &low_to_high_bitmask,
                                  &high_to_low_bitmask,
                                  _gpiote_event_handler);
    if (ret != NRF_SUCCESS)
        return false;
    
    ret = app_gpiote_user_enable(_gpiote_user_id);
    
    return ret == NRF_SUCCESS;
}

void pxi_nrf_gpio_in_set_interrupt_handler(pxi_nrf_gpio_interrupt_handle handler)
{
    _gpio_interrupt_handler = handler;
}

void pxi_nrf_gpio_in_pull(pxi_nrf_gpio_in gpio_in, pxi_nrf_gpio_pull_type pull_type)
{
    if (pull_type == PXI_NRF_GPIO_PULL_TYPE_NOPULL)
    {
        nrf_gpio_pin_sense_t sense = pxi_nrf_gpio_in_is_high(gpio_in) ? NRF_GPIO_PIN_SENSE_LOW : NRF_GPIO_PIN_SENSE_HIGH;
        
        nrf_gpio_cfg_input(gpio_in, NRF_GPIO_PIN_NOPULL);
        nrf_gpio_cfg_sense_set(gpio_in, sense);
    }
    else if (pull_type == PXI_NRF_GPIO_PULL_TYPE_PULLDOWN)
    {
        nrf_gpio_cfg_input(gpio_in, NRF_GPIO_PIN_PULLDOWN);
        nrf_gpio_cfg_sense_set(gpio_in, NRF_GPIO_PIN_SENSE_HIGH);
    }
    else if (pull_type == PXI_NRF_GPIO_PULL_TYPE_PULLUP)
    {
        nrf_gpio_cfg_input(gpio_in, NRF_GPIO_PIN_PULLUP);
        nrf_gpio_cfg_sense_set(gpio_in, NRF_GPIO_PIN_SENSE_LOW);
    }
}

bool pxi_nrf_gpio_in_is_high(pxi_nrf_gpio_in gpio_in)
{
    uint32_t gpio_states = 0;
    uint32_t ret = 0;
    
    ret = app_gpiote_pins_state_get(_gpiote_user_id, &gpio_states);
    if (ret != NRF_SUCCESS)
        return false;
    
    return IS_SET(gpio_states, gpio_in);
}

bool pxi_nrf_gpio_out_enable(pxi_nrf_gpio_out gpio_out)
{
    nrf_gpio_cfg_output((uint32_t)gpio_out);
    return true;
}

void pxi_nrf_gpio_out_toggle(pxi_nrf_gpio_out gpio_out)
{
    nrf_gpio_pin_toggle((uint32_t)gpio_out);
}

void pxi_nrf_gpio_out_write(pxi_nrf_gpio_out gpio_out, bool high)
{
    nrf_gpio_pin_write((uint32_t)gpio_out, high ? 1 : 0);
    
    //// Another implementation
    //if (high)
    //    nrf_gpio_pin_set((uint32_t)gpio_out);
    //else
    //    nrf_gpio_pin_clear((uint32_t)gpio_out);
}

bool pxi_nrf_gpio_out_is_high(pxi_nrf_gpio_out gpio_out)
{
    return ((NRF_GPIO->OUT >> gpio_out) & 1UL);
}
