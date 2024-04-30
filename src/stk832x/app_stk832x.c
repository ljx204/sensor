/*
    liujian define

		2024/4/24

*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "app_stk832x.h"

#include "stk832x.h"




#define G_INT1         17
#define G_INT2         5


static void stk8321_IRQ_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

static void stk8321_irq_init(void)
{
    ret_code_t err_code;
	
    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config.pull = NRF_GPIO_PIN_PULLUP;
	  in_config.sense = NRF_GPIOTE_POLARITY_TOGGLE;
	

    err_code = nrf_drv_gpiote_in_init(G_INT1, &in_config, stk8321_IRQ_handler);
    APP_ERROR_CHECK(err_code);
	
		nrf_drv_gpiote_in_event_enable(G_INT1, true);
	
//		nrf_gpio_cfg_sense_input(G_INT1,
//                             NRF_GPIO_PIN_PULLUP,
//                             NRF_GPIO_PIN_SENSE_HIGH);
//	
//		nrf_gpio_cfg_output(SMARTDEVICE_GPIO_LED_RUN);
//	
//    nrf_gpio_pin_write(SMARTDEVICE_GPIO_LED_RUN, 0);
	
	
//		nrf_drv_gpiote_in_config_t in_config2 = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);//NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
//    in_config2.pull = NRF_GPIO_PIN_PULLUP;

//    err_code = nrf_drv_gpiote_in_init(POWER_KEY, &in_config2, powerkey_IRQ_handler);
//    APP_ERROR_CHECK(err_code);
//		nrf_drv_gpiote_in_event_enable(POWER_KEY, true);
		
}

void app_stk832x_init(void)
{
	
//	   stk832x_init();
//	
//	   stk8321_irq_init();
	
}


static void stk8321_IRQ_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{     
	uint8_t data[13] ;
	uint16_t xyz[3];
	uint16_t len = 12;
	uint64_t mSec = 0;
	
	NRF_LOG_INFO("stk8321_IRQ_handler");

}









