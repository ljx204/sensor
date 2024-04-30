/*

   liujian
	 
	 ctd_liu@163.com
	 
   ĿǰADC �����ӵ� AIN5
	 
*/
#include "app_saadc.h"


#define  SAMPLES_IN_BUFFER       (1)
#define  ADC_SAMPLE_BUFFER_SIZE  (1)

volatile uint8_t state = 1;

APP_TIMER_DEF(saadc_timer_id);

static const nrf_drv_timer_t m_timer = NRF_DRV_TIMER_INSTANCE(2);

uint8_t BatteryVoltage;

uint8_t BatteryPercent;

static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];

static nrf_ppi_channel_t     m_ppi_channel;

nrf_saadc_value_t adc_buffer[2][ADC_SAMPLE_BUFFER_SIZE];

void saadc_timer_init(void);
void saadc_timer_start(void);
void saadc_timer_stop(void);

void timer_handler(nrf_timer_event_t event_type, void * p_context)
{

}

//void adc_callback(nrfx_saadc_evt_t const * p_event)
//{
//     if( p_event->type == NRFX_SAADC_EVT_DONE ){
//         nrfx_saadc_buffer_convert(p_event->data.done.p_buffer, ADC_SAMPLE_BUFFER_SIZE);
//         uint16_t adc = p_event->data.done.p_buffer[0];  
//			 
//  //       BatteryVoltage = (uint16_t)(((uint32_t)adc * 2 * 6 * 6000) / (1024 * 10));
//			 
//	//		 SEGGER_RTT_printf(0,"bat222 %d",batval);
////         SmartDevice_BatteryProcess(BatteryVoltage); 
//	//		 SEGGER_RTT_printf(0,"BatteryVoltage %d\r\n",BatteryVoltage);
//			// nrf_drv_saadc_uninit();
//					
//					
//			 //		SEGGER_RTT_printf(0,"bat %d",BatteryVoltage);
//    }
//}

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
			  uint32_t   adc_battery = 0;
        ret_code_t err_code;

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);

        for (int i = 0; i < SAMPLES_IN_BUFFER; i++)
        {
					  adc_battery += p_event->data.done.p_buffer[i];
        }
				
			//	adc_battery >>= 3;
				
				BatteryVoltage = (uint16_t)(((uint32_t)adc_battery * 2 * 6 * 6000) / (1024 * 10));
    }
}


void app_saadc_sampling_event_enable(void)
{
    ret_code_t err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);
}

void app_saadc_sampling_event_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&m_timer, &timer_cfg, timer_handler);
    APP_ERROR_CHECK(err_code);

    /* setup m_timer for compare event every 400ms */
    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer, 400);
    nrf_drv_timer_extended_compare(&m_timer,
                                   NRF_TIMER_CC_CHANNEL0,
                                   ticks,
                                   NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                   false);
    nrf_drv_timer_enable(&m_timer);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer,
                                                                                NRF_TIMER_CC_CHANNEL0);
    uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel,
                                          timer_compare_event_addr,
                                          saadc_sample_task_addr);
    APP_ERROR_CHECK(err_code);
}


void app_saadc_init(void)
{
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN5);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);
	
	  app_saadc_sampling_event_init();
	
    app_saadc_sampling_event_enable();
	
}
