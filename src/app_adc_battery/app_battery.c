/*
        liujian
				
        ctd_liu@163.com

        2024/5/11
*/

#include "app_battery.h"
#include "app_pah8009_v1.h"
#include "pah800x_main.h"
#include "app_timer.h"
#include "nrf_gpio.h"

#define BATTER_CHG_DET     31
#define BATTER_CHG_DONE    30

APP_TIMER_DEF(batter_state_timer);
APP_TIMER_DEF(delay_batter_state_timer);


#define BATTER_CHECK_TIME     APP_TIMER_TICKS(1000)

static unsigned char chg_det_debounce = 0;
static unsigned char chg_done_debounce = 0;

CHARGE_STATE state_charge = BATTER_NO_CHARGE;

static void app_battery_led_state_func(void *context)
{
	   LED_TYPE led = (LED_TYPE)(context);
	
	   pah8009_set_led(led);
}

static void app_battery_state_func(void *context)
{
	    if(nrf_gpio_pin_read(BATTER_CHG_DET) == 0)
			{
				  chg_det_debounce++;
				
				  if(chg_det_debounce > 2)
					{
						 if(state_charge != BATTERY_CHARGEING)
						 {
						     app_pah8009_stop();
					
						     state_charge = BATTERY_CHARGEING;
							 
							   app_timer_start(batter_state_timer, APP_TIMER_TICKS(1000), (void *)LED_RED);							   
						 }
					}
				//	NRF_LOG_INFO("LOW");
			}
			else
			{
				  chg_det_debounce = 0;
				  if(state_charge != BATTER_NO_CHARGE)
				  {
				     state_charge = BATTER_NO_CHARGE;
						
						 pah_sensor_init(diw_4mm_g_ir_hrd);      //heartbeat
	
          // pah_sensor_init(diw_4mm_ir_r_ir_spo2);  //spo2
						
              auto_mode_sensor_start();
		
					}
				//	NRF_LOG_INFO("HIGH");
			}
			
			if(nrf_gpio_pin_read(BATTER_CHG_DONE) == 0)
			{
				  chg_done_debounce++;
				
				  if(chg_done_debounce > 2)
					{
						 //pah8009_set_led(LED_GREEN);
						 app_timer_start(batter_state_timer, APP_TIMER_TICKS(1000), (void *)LED_GREEN);
						
						 state_charge = BATTERY_CHARGE_DONE;
					}
			}
			else
			{
				  chg_done_debounce = 0;
			}
}


void app_battery_state(void)
{

    uint32_t error_code;

    error_code = app_timer_create(&batter_state_timer, APP_TIMER_MODE_REPEATED, app_battery_state_func);
	
    APP_ERROR_CHECK(error_code);
	
	  error_code = app_timer_create(&delay_batter_state_timer, APP_TIMER_MODE_SINGLE_SHOT, app_battery_led_state_func);
	
    APP_ERROR_CHECK(error_code);
	
	
	  nrf_gpio_cfg_input(BATTER_CHG_DET, NRF_GPIO_PIN_PULLUP);
	
	  nrf_gpio_cfg_input(BATTER_CHG_DONE, NRF_GPIO_PIN_PULLUP);
	
	  app_timer_start(batter_state_timer, BATTER_CHECK_TIME, NULL);
	
}

