/*
     Liujian 
		 
		 2024/4/23

*/
#ifndef __APP_UART_TASK_H__
#define __APP_UART_TASK_H__

#include "app_error.h"
#include "ble_uart.h"
#include "bd_communicate_protocol.h"

void app_uart_service_init(void);

uint32_t app_uart_data_send(ble_uart_t * p_uart, uint8_t  * p_data, uint16_t  *p_length, uint16_t conn_handle);


uint32_t ble_uart_send_string(uint8_t * string, uint16_t *length);

void  app_button_send_data(void);


void app_change_ble_addr(void);


void set_complete_callback(SendCompletePara para);


bool app_uart_send_ready(void);

#endif




