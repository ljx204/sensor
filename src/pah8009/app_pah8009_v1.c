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



extern void ppg_sensor_interrupt_process(void *p_event_data, uint16_t event_size);

void ppg_handle_gpio_interrupt(pxi_nrf_gpio_in gpio_in, pxi_nrf_gpio_status gpio_status)
{

    if (gpio_in == PXI_NRF_GPIO_IN_6&& gpio_status == PXI_NRF_GPIO_STATUS_LOW)
    {
#if PPG_SCHEDULER_ENABLE	
         app_sched_event_put(NULL, 0,  ppg_sensor_interrupt_process);
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
	
	   ret = pah_sensor_init(diw_4mm_g_ir_hrd);
	
	   if(!ret)
		 {
			   return;
		 }
	
}





