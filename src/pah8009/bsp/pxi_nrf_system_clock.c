/*============================================================================
* To enable system clock
*
* 1. Open "nrf_drv_config.h"
*  // TIMER
*  #define TIMER0_ENABLED 1
*
* 2. Add header search path
*  \components\drivers_nrf\timer
*
* 3. Add bsp source files
*  \components\drivers_nrf\timer\nrf_drv_timer.c
*
============================================================================*/

#include "pxi_nrf_system_clock.h"

// bsp
#include <nrf_drv_timer.h>
#include <nrf_delay.h>
/*-----------------------------rtc-----------------------------*/
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0); /**< Declaring an instance of nrf_drv_rtc for RTC0. */
extern void rtc_handler(nrf_drv_rtc_int_type_t int_type);
static void lfclk_config(void);
static void rtc_config(void);
/*-----------------------------rtc-----------------------------*/

//============================================================================
// Private global variables
//============================================================================
static const nrf_drv_timer_t timer_handle = NRF_DRV_TIMER_INSTANCE(1);
static volatile uint64_t tick_count = 0;


//============================================================================
// Private functions
//============================================================================
static void timer_led_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            ++tick_count;
            break;
        
        default:
            //Do nothing.
            break;
    }    
}

///*--------------------------------------rtc--------------------------------------*/
///** @brief Function starting the internal LFCLK XTAL oscillator.
// */
//static void lfclk_config(void)
//{
//    ret_code_t err_code = nrf_drv_clock_init();
//    APP_ERROR_CHECK(err_code);

//    nrf_drv_clock_lfclk_request(NULL);
//}

/** @brief Function initialization and configuration of RTC driver instance.
 */
//static void rtc_config(void)
//{
//    uint32_t err_code;

//    //Initialize RTC instance
////    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
////    config.prescaler = 4095;
//    err_code = nrf_drv_rtc_init(&rtc, NULL, rtc_handler);
//    APP_ERROR_CHECK(err_code);

//    //Enable tick event & interrupt
//    nrf_drv_rtc_tick_enable(&rtc, true);

//    //Set compare channel to trigger interrupt after COMPARE_COUNTERTIME seconds
//    //err_code = nrf_drv_rtc_cc_set(&rtc, 0, COMPARE_COUNTERTIME*RTC0_CONFIG_FREQUENCY, true);
//    //APP_ERROR_CHECK(err_code);

//    //Power on RTC instance
//    nrf_drv_rtc_enable(&rtc);
//}
/*--------------------------------------rtc--------------------------------------*/


//============================================================================
// Public functions
//============================================================================
bool pxi_nrf_system_clock_init(void)
{
    uint32_t time_ms = 1; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;
	
//	  nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
// //   err_code = nrf_drv_timer_init(&TIMER_LED, &timer_cfg, timer_led_event_handler);
//	
//    
//    //Configure timer_handle for generating simple light effect - leds on board will invert his state one after the other.
//    err_code = nrf_drv_timer_init(&timer_handle, &timer_cfg, timer_led_event_handler);
//    APP_ERROR_CHECK(err_code);
	
	  err_code = nrf_drv_timer_init(&timer_handle, NULL, timer_led_event_handler);
    APP_ERROR_CHECK(err_code);
	
    
    time_ticks = nrf_drv_timer_ms_to_ticks(&timer_handle, time_ms);
    
    nrf_drv_timer_extended_compare(&timer_handle, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    
    nrf_drv_timer_enable(&timer_handle);
    
	 return true;
}

uint64_t pxi_nrf_get_tick_count(void)
{
    return tick_count;
}

//void pxi_nrf_delay_ms(uint32_t ms)
//{
//    uint32_t start_cnt = tick_count;
//	while((tick_count - start_cnt) < ms)
//    {
//    }
//    
//    //// Another implementation. 0.05% error. nrf_delay_ms(1000) but tick_count increase 950, 
//	//nrf_delay_ms(ms);
//}

//bool pxi_nrf_rtc_init(void)
//{
//    lfclk_config();

//    rtc_config();

//    return true;
//}
