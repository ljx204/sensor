/*
  2024/4/30

  liujian

  ctd_liu@163.com

  deal with phone data and report data to phone

*/

#ifndef __APP_PROCESS_DATA_H__
#define __APP_PROCESS_DATA_H__

#include <stdint.h>

void app_process_data_init(void);

void battery_capacity_report(uint8_t key);

void device_information_report(uint8_t key);

void battery_charge_state_report(uint8_t key);


#endif
