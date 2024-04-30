/*
    liujian
		
		2024/4/26

*/

#include "app_ble.h"
#include "app_gatt.h"

#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"


#define DEVICE_NAME                     "co8"                                /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME               "RDW_LIUJIAN"                           /**< Manufacturer. Will be passed to Device Information Service. */



#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)        /**< Minimum acceptable connection interval (0.1 seconds). */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)        /**< Maximum acceptable connection interval (0.2 second). */
#define SLAVE_LATENCY                   0                                       /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)         /**< Connection supervisory timeout (4 seconds). */



NRF_BLE_GATT_DEF(m_gatt);          /**< GATT module instance. */

/**@brief Function for initializing the GATT module.
 */
static void gatt_init(void)
{
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void)
{
    ret_code_t              err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;
	
	  ble_gap_addr_t   addr;
    char name[BLE_GAP_DEVNAME_MAX_LEN];
    uint8_t tmp;
	
    err_code = sd_ble_gap_addr_get(&addr);
	
    memset(name, 0, sizeof(name));
	
	  strcpy(name, DEVICE_NAME); 
	  strcat(name, "_");   
	
	  tmp = (addr.addr[4] >> 4 );    
	  tmp = tmp>=10?(tmp + 0x37):(tmp+0x30);    
	  strncat(name,(char *)&tmp,1);
	
	  tmp = addr.addr[4] % 0x10;    
		tmp = (tmp >= 10) ? (tmp + 0x37):(tmp + 0x30);   
		strncat(name, (char *)&tmp, 1); 
		
		tmp = (addr.addr[3] >> 4 );    
	  tmp = tmp>=10?(tmp + 0x37):(tmp+0x30);    
	  strncat(name,(char *)&tmp,1);
	
	  tmp = addr.addr[3] % 0x10;    
		tmp = (tmp >= 10) ? (tmp + 0x37):(tmp + 0x30);   
		strncat(name, (char *)&tmp, 1); 
		
		tmp = (addr.addr[2] >> 4 );    
	  tmp = tmp>=10?(tmp + 0x37):(tmp+0x30);    
	  strncat(name,(char *)&tmp,1);
	
	  tmp = addr.addr[2] % 0x10;    
		tmp = (tmp >= 10) ? (tmp + 0x37):(tmp + 0x30);   
		strncat(name, (char *)&tmp, 1); 
	
	  
	  tmp = (addr.addr[1] >> 4 );    
	  tmp = tmp>=10?(tmp + 0x37):(tmp+0x30);    
	  strncat(name,(char *)&tmp,1); 
    
		tmp = addr.addr[1] % 0x10;    
		tmp = (tmp >= 10) ? (tmp + 0x37):(tmp + 0x30);   
		strncat(name, (char *)&tmp, 1); 
    
		tmp = (addr.addr[0]>>4);   
		tmp = (tmp >= 10)?(tmp+0x37):(tmp+0x30);   
		strncat(name, (char *)&tmp, 1);     
		
		tmp = addr.addr[0] % 0x10;
		tmp = tmp >=10 ? (tmp + 0x37):(tmp + 0x30);    
		strncat(name, (char *)&tmp, 1); 
		
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)name,
                                          strlen(name));
    APP_ERROR_CHECK(err_code);


    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

void app_gatt_init(void)
{
	  gap_params_init();
    gatt_init();
}

