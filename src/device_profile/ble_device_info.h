/*
    liujian define
		
		2024/4/28

*/

#ifndef __BLE_DEVICE_INFO_H__
#define __BLE_DEVICE_INFO_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_DEVICE_BLE_OBSERVER_PRIO 2

#define DEVICE_UUID_SERVICE        0xFE00
#define DEVICE_UUID_READ_MAC       0xFE01
#define DEVICE_UUID_READ_SN        0xFE02
#define DEVICE_UUID_READ_INFO      0xFE03       

/**@brief Device Information Service init structure. This contains all possible characteristics
 *        needed for initialization of the service.
 */
typedef struct
{
    ble_srv_utf8_str_t             device_mac;           /**< Manufacturer Name String. */
    ble_srv_utf8_str_t             device_sn;               /**< Model Number String. */
    ble_srv_utf8_str_t             device_info;              /**< Serial Number String. */
	
//    ble_srv_utf8_str_t             hw_rev_str;                  /**< Hardware Revision String. */
//    ble_srv_utf8_str_t             fw_rev_str;                  /**< Firmware Revision String. */
//    ble_srv_utf8_str_t             sw_rev_str;                  /**< Software Revision String. */
//    ble_dis_sys_id_t *             p_sys_id;                    /**< System ID. */
//    ble_dis_reg_cert_data_list_t * p_reg_cert_data_list;        /**< IEEE 11073-20601 Regulatory Certification Data List. */
//    ble_dis_pnp_id_t *             p_pnp_id;                    /**< PnP ID. */
      security_req_t                 dis_char_rd_sec;             /**< Security requirement for reading any DIS characteristic value. */
} ble_device_info_init_t;



uint32_t ble_device_info_init(ble_device_info_init_t const * p_device_init);

#ifdef __cplusplus
}
#endif


#endif
