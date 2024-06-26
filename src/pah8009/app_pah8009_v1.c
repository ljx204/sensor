/*
        liujian
		
		    2024/4/26

*/

#include "app_pah8009_v1.h"

#include "pah_800x.h"
#include "pah800x_bsp.h"
#include "pah800x_main.h"

#include "app_scheduler.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

static PAH8009_WORK_MODE  pah_work_mode;

static void pah_write_data(unsigned char reg, const unsigned char data)
{
	 pah_comm_i2c_write(reg, &data, 1);
}

/*
  led_type = 0  //infrared
  led_type = 1  //green
  led_type = 2;  //red
*/
void pah8009_set_led(LED_TYPE led_type)
{

	  pah_write_data(0xE8, 0x00);
	  nrf_delay_ms(20);
	  pah_write_data(0xEE, 0x1f);
	  nrf_delay_ms(20);
	  pah_write_data(0x03, 0x40);
	  pah_write_data(0x04, 0x01);
	  pah_write_data(0x05, 0x00);
	  pah_write_data(0x11, 0x11);
	
	  if(led_type == LED_INFRARED)
		{
	     pah_write_data(0x1d, 0x1f);
		}
		else if(led_type == LED_GREEN)
		{
			 pah_write_data(0x1d, 0x2f);
		}
		else if(led_type == LED_RED)
		{
			  pah_write_data(0x1d, 0x4f);
		}
	
	  
	  pah_write_data(0xEB, 0x80);
	  nrf_delay_ms(20);
	  pah_write_data(0xE8, 0x00);
	  nrf_delay_ms(20);
	  pah_write_data(0x07, 0x01);

}

void pah8009_led_off(void)
{
	
	   pah_write_data(0xE8, 0x00);
	   pah_write_data(0xEE, 0x1F);
}



extern void ppg_sensor_interrupt_process_hr(void *p_event_data, uint16_t event_size);

extern void ppg_sensor_interrupt_process_spo2(void *p_event_data, uint16_t event_size);

void ppg_handle_gpio_interrupt(pxi_nrf_gpio_in gpio_in, pxi_nrf_gpio_status gpio_status)
{

    if (gpio_in == PXI_NRF_GPIO_IN_6 && gpio_status == PXI_NRF_GPIO_STATUS_LOW)
    {
#if PPG_SCHEDULER_ENABLE
//		    _main.interrupt_timestamp = pah_get_tick_count();
//	
//         process(_main.interrupt_timestamp);
			
	//		   ppg_process();
			
			  if(pah_work_mode == HEARTBEAT_MODE)
				{
					//ppg_sensor_interrupt_process_hr(NULL, 0);
           app_sched_event_put(NULL, 0, ppg_sensor_interrupt_process_hr);
				}
				else if(pah_work_mode == SPO2_MODE)
				{
					 app_sched_event_put(NULL, 0, ppg_sensor_interrupt_process_spo2);
				}
#else
        ppg_interrupt_handle();
#endif
    }
}


void app_pah8009_process(void)
{
	   ret_code_t err_code;
	
	   int ret;
	
	   pxi_nrf_system_clock_init();

     pxi_nrf_gpio_in_init();
     pxi_nrf_gpio_in_set_interrupt_handler(ppg_handle_gpio_interrupt);
     pxi_nrf_gpio_in_pull(PXI_NRF_GPIO_IN_6, PXI_NRF_GPIO_PULL_TYPE_PULLUP);
	
	   app_pah8009_start(HEARTBEAT_MODE);
	
//	   ret = pah_sensor_init(diw_4mm_g_ir_hrd);
//	
//	   if(!ret)
//		 {
//			   return;
//		 }
	
}

void app_pah8009_start(PAH8009_WORK_MODE mode)
{
	  int ret;
	
	  pxi_nrf_gpio_in_set_interrupt_handler(ppg_handle_gpio_interrupt);
	
	  if(mode == HEARTBEAT_MODE)
		{
	      ret = pah_sensor_init(diw_4mm_g_ir_hrd);       //heartbeat
		}
		else if(mode == SPO2_MODE)
		{
        ret = pah_sensor_init(diw_4mm_ir_r_ir_spo2);   //spo2
	  }

    if (!ret)
    {
        return;
    }
		
		pah_work_mode = mode;
		
    auto_mode_sensor_start();
}

void app_pah8009_stop(void)
{
	  pxi_nrf_gpio_in_set_interrupt_handler(NULL);
	  hr_algorithm_disable();
	  spo2_algorithm_disable();
	  pah_sensor_stop();
}






