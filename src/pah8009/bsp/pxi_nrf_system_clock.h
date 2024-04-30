#pragma once


#include <stdbool.h>
#include <stdint.h>

#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"

//============================================================================
// Public functions
//============================================================================
bool        pxi_nrf_system_clock_init(void);
uint64_t    pxi_nrf_get_tick_count(void);
void        pxi_nrf_delay_ms(uint32_t ms);
bool 		pxi_nrf_rtc_init(void);

