

#ifndef __APP_PAH8009_H__
#define __APP_PAH8009_H__

typedef enum
{
	LED_INFRARED,
	LED_GREEN,
	LED_RED,

} LED_TYPE;





int app_pah8009_init(void);

void app_pah8009_process(void);

void pah8009_set_led(LED_TYPE led_type);

void pah8009_led_off(void);

void app_pah8009_stop(void);

void app_pah8009_start(PAH8009_WORK_MODE mode);

#endif
