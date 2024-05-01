/*
    liujian define

    2024/4/24

*/

#ifndef __APP_H__
#define __APP_H__

#include "app_uart_task.h"
#include "button.h"
#include "ble_dfu.h"

#define SCHED_MAX_EVENT_DATA_SIZE 10
#define SCHED_QUEUE_SIZE 10

void app_init(void);

#endif
