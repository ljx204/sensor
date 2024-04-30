/*
  2024/4/30
	
	liujian
	
	ctd_liu@163.com
	
  deal with phone data and report data to phone

*/

#include "app_process_data.h"
#include <stdint.h>
#include <nrf_assert.h>
#include "app_timer.h"
#include "app_scheduler.h"
#include "bd_communicate_protocol.h"
#include "nordic_common.h"
#include "pxialg.h"
#include "app_motion.h"
#include "app_uart_task.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


APP_TIMER_DEF(m_data_report_timer_id);

static void data_report_timer_handle(void * val);
	
void app_process_data_init(void)
{
	  uint32_t err_code;
	
    err_code = app_timer_create(&m_data_report_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                data_report_timer_handle);
	
	  app_timer_start(m_data_report_timer_id, APP_TIMER_TICKS(10000), NULL);
	
}

void send_all_data_callback(SEND_STATUS status)
{

    if(status != SEND_SUCCESS) {
        NRF_LOG_INFO("send faile\n");       
    }
}

uint8_t sync_process_buffer[8];

static bool send_sport_data_sync_start( )
{

    uint32_t  ret = NRF_SUCCESS;
	
	  uint32_t  step;
	
	  float heart_rate;

    L2_Send_Content sendContent;
	
	  PxiAlg_HrGet(&heart_rate);
	
	  step = stk832x_getstep();

    sync_process_buffer[0] = HEALTH_DATA_COMMAND_ID; //L2 commandID
    sync_process_buffer[1] = L2_HEADER_VERSION; //L2 commandID
    sync_process_buffer[2] = KEY_DATA_SYNC_START; //key
    sync_process_buffer[3] = (unsigned char)heart_rate;
    sync_process_buffer[4] = step&0xff;
	  sync_process_buffer[5] = (step>>8)&0xff;
	  sync_process_buffer[6] = (step>>16)&0xff;
	  sync_process_buffer[7] = (step>>24)&0xff;


    sendContent.callback  = send_all_data_callback;
    sendContent.content  = (uint8_t *)sync_process_buffer;
    sendContent.length  = 8;
    ret = L1_send(&sendContent);
    if(ret == NRF_SUCCESS) {
        return true;
    } else {
        return false;
    }

}

static void data_report_process(void *p_event_data, uint16_t event_size)
{
	     if(app_uart_send_ready())
			 {
          send_sport_data_sync_start();
			 }

}

static void data_report_timer_handle(void * val)
{
	 app_sched_event_put(NULL, 0,  data_report_process);

}