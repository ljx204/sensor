

#ifndef __APP_PAH8009_V1_H__
#define __APP_PAH8009_V1_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum {
	  LED_INFRARED,
	  LED_GREEN,
	  LED_RED,	
}LED_TYPE;


typedef enum {
	  HEARTBEAT_MODE,
	  SPO2_MODE
}PAH8009_WORK_MODE;

void pah8009_set_led(LED_TYPE led_type);
void pah8009_led_off(void);

void app_pah8009_start(PAH8009_WORK_MODE mode);

void app_pah8009_stop(void);

void app_pah8009_process(void);

#endif

