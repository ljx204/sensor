/*
  2024/4/30

  liujian

  ctd_liu@163.com

  deal with phone data and report data to phone

*/

#ifndef __APP_PROCESS_DATA_H__
#define __APP_PROCESS_DATA_H__

#include <stdint.h>

typedef struct {
	
	 uint16_t  year;
	
	 uint8_t   month;
	
	 uint8_t   day;
	
	 uint8_t   hour;
	
	 uint8_t   minute;
	
	 uint8_t   second;
	
}watch_time;

typedef struct {
	  uint8_t    sex;
	  
	  uint8_t    age;
	
	  uint16_t   height;
	
	  uint16_t   weight;
	
	
}ring_profile;


void app_process_data_init(void);

void battery_capacity_report(uint8_t key);

void device_information_report(uint8_t key);

void battery_charge_state_report(uint8_t key);

void ring_device_time_setting(uint8_t *p_buffer);

void ring_device_profile_setting(uint8_t *p_buffer);

#endif
