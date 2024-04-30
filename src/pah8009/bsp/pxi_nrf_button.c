#include "pxi_nrf_button.h"

// bsp
#include <bsp.h>
#include <app_button.h>
#include <app_timer.h>

// c
#include <stddef.h>


//============================================================================
// Macro
//============================================================================
#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof(arr[0]))


//============================================================================
// Private global variables
//============================================================================
#define APP_TIMER_PRESCALER      0                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE  2                           /**< Size of timer operation queues. */

#define BUTTON_DETECTION_DELAY        APP_TIMER_TICKS(50u, APP_TIMER_PRESCALER) /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

static void _button_event_handler(uint8_t pin_no, uint8_t button_action);
static const app_button_cfg_t app_buttons[] =
{
    /*
    #ifdef BSP_BUTTON_0
    {BSP_BUTTON_0, false, BUTTON_PULL, _button_event_handler},
    #endif // BUTTON_0
    
    #ifdef BSP_BUTTON_1
    {BSP_BUTTON_1, false, BUTTON_PULL, _button_event_handler},
    #endif // BUTTON_1
    */
    
    #ifdef BSP_BUTTON_2
    {BSP_BUTTON_2, false, BUTTON_PULL, _button_event_handler},
    #endif // BUTTON_2
    
    
    #ifdef BSP_BUTTON_3
    {BSP_BUTTON_3, false, BUTTON_PULL, _button_event_handler},
    #endif // BUTTON_3
    
};

static button_click_handle _button_click_handler = NULL;


//============================================================================
// Private functions
//============================================================================
static void _button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    if (!_button_click_handler)
        return;
    
    pxi_nrf_button_id button_id;
    if (pin_no == BSP_BUTTON_2)
        button_id = PXI_NRF_BUTTON_3;
    else if (pin_no == BSP_BUTTON_3)
        button_id = PXI_NRF_BUTTON_4;
    else
        return;
    
    pxi_nrf_button_status button_status = (button_action == APP_BUTTON_RELEASE) ? PXI_NRF_BUTTON_RELEASE : PXI_NRF_BUTTON_PUSH;
    _button_click_handler(button_id, button_status);
}

static void clock_initialization(void)
{
    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;

    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        // Do nothing.
    }
}


//============================================================================
// Public functions
//============================================================================
bool pxi_nrf_button_init(void)
{
    uint32_t err_code = NRF_SUCCESS;

    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, NULL);
    
    clock_initialization();
    
    err_code = app_button_init((app_button_cfg_t*)app_buttons, ARRAY_SIZE(app_buttons), BUTTON_DETECTION_DELAY);
    if (err_code != NRF_SUCCESS)
        return false;
    
    err_code = app_button_enable();
    if (err_code != NRF_SUCCESS)
        return false;
    
    return true;
    
}

void pxi_nrf_button_set_callback(button_click_handle handler)
{
    _button_click_handler = handler;
}
