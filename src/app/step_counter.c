

#include "step_counter.h"
#include "nordic_common.h"
//#include "click-algorithm.h"

static bool sensor_timer_status = false;

APP_TIMER_DEF(m_sensor_timer_id);

#define SENSOR_INTERVAL      APP_TIMER_TICKS(280)   /**< Battery level measurement interval (ticks). */


void sensor_timer_create(void)
{
	  uint32_t err_code;
	
    err_code = app_timer_create(&m_sensor_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                sensor_timer_handle);
    APP_ERROR_CHECK(err_code);
}

void sensor_timers_start(void)
{
    uint32_t               err_code;
    if(!sensor_timer_status) {

#if defined(DEBUG_LOG) || defined (DEBUG_ACC)
        LOG(LEVEL_INFO,">>startSensor:  %d\r\n",mSleep_mode);
#endif

        err_code = app_timer_start(m_sensor_timer_id, SENSOR_INTERVAL, NULL);
        APP_ERROR_CHECK(err_code);

        sensor_timer_status = true;
//        sensor_started_minutes = 0;
    }
}

void sensor_timers_stop(void)
{
    uint32_t   err_code;
    if(sensor_timer_status) {

#if defined(DEBUG_LOG) || defined (DEBUG_ACC)
        LOG(LEVEL_INFO," stop sensor timer \r\n");
#endif

        err_code = app_timer_stop(m_sensor_timer_id);
        APP_ERROR_CHECK(err_code);
        sensor_timer_status = false;
//        sensor_started_minutes = 0;
    }
}


void sensor_timer_handle(void * val)
{
  

}

