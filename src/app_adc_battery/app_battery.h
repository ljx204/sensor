/*
        liujian
				
        ctd_liu@163.com

        2024/5/11
*/

#ifndef _APP_BATTER_H__
#define _APP_BATTER_H__


typedef enum {
     BATTER_NO_CHARGE,
     BATTERY_CHARGEING,	
	   BATTERY_CHARGE_DONE	
}CHARGE_STATE;

extern CHARGE_STATE state_charge;

void app_battery_state(void);


#endif
