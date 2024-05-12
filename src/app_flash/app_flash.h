/*
    liujian

    ctd_liu@163.com

    2024/5/12
*/

#ifndef __APP_FLASH_H__
#define __APP_FLASH_H__

#include <stdint.h>
#include <stdbool.h>

void app_flash_init(void);


#define CONFIG_FILE     (0x8010)
#define CONFIG_REC_KEY  (0x7010)


/* A dummy structure to save in flash. */
typedef struct
{
    uint32_t boot_count;
    char     device_name[16];
    bool     config1_on;
    bool     config2_on;
} configuration_t;


#endif

