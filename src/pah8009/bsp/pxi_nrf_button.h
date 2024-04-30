#pragma once


// c
#include <stdint.h>
#include <stdbool.h>


//============================================================================
// Definitions
//============================================================================
typedef enum
{
    //PXI_NRF_BUTTON_1 = 1,     // Not used because collision with gpio_in
    //PXI_NRF_BUTTON_2 = 2,     // Not used because collision with gpio_in
    PXI_NRF_BUTTON_3 = 3,
    PXI_NRF_BUTTON_4 = 4,
    
    PXI_NRF_BUTTON_NUM
    
} pxi_nrf_button_id;

typedef enum
{
    PXI_NRF_BUTTON_PUSH,
    PXI_NRF_BUTTON_RELEASE,
} pxi_nrf_button_status;


typedef void (*button_click_handle)(pxi_nrf_button_id button_id, pxi_nrf_button_status button_status);


//============================================================================
// Public functions
//============================================================================
bool    pxi_nrf_button_init(void);
void    pxi_nrf_button_set_callback(button_click_handle handler);

