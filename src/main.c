/**
 * Copyright (c) 2014 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 
   liujian modified
	 
	 ctd_liu@163.com
	 
	 2024/4/26
	 
*/

#include "main.h"
#include "app.h"
#include "app_ble.h"
#include "app_scheduler.h"


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}



/**@brief Function for application main entry.
 */
int main(void)
{
	
	  app_init();
	
    advertising_start(true);

    // Enter main loop.
    for (;;)
    {
#if PPG_SCHEDULER_ENABLE			
			  app_sched_execute();
#else
			  ppg_loop();
#endif			
        idle_state_handle();
				  
    }
}

