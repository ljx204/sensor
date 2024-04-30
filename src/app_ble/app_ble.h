

#ifndef __APP_BLE_H__
#define __APP_BLE_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"

#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"

#include "app_timer.h"

/**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */
#define APP_ADV_INTERVAL      300     

/**< The advertising duration (180 seconds) in units of 10 milliseconds. */
#define APP_ADV_DURATION      18000   

/**< A tag identifying the SoftDevice BLE configuration. */
#define APP_BLE_CONN_CFG_TAG            1                                     


#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)      /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)     /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                          /**< Number of attempts before giving up the connection parameter negotiation. */



void app_dfu_advertising_config_set(ble_adv_modes_config_t const * const p_adv_modes_config);

void app_ble_init(void);


void advertising_start(bool erase_bonds);

#endif
