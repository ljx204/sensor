/*
    liujian define
		
		2024/4/23

*/

#include "sdk_common.h"
#include "ble.h"
#include "ble_uart.h"
#include "nrf_log.h"
#include "ble_srv_common.h"

#define BLE_UART_MAX_RX_CHAR_LEN        BLE_UART_MAX_DATA_LEN /**< Maximum length of the RX Characteristic (in bytes). */
#define BLE_UART_MAX_TX_CHAR_LEN        BLE_UART_MAX_DATA_LEN /**< Maximum length of the TX Characteristic (in bytes). */


uint32_t ble_uart_init(ble_uart_t * p_uart, const ble_uart_init_t * p_uart_init)
{
    uint32_t              err_code;
    ble_uuid_t            ble_uuid;
    ble_add_char_params_t add_char_params;

    // Initialize service structure.
    p_uart->ble_data_rx_handler     = p_uart_init->uart_data_handler;
	  p_uart->ble_tx_complete_handler = p_uart_init->uart_tx_complete_handler;

    // Add service.

    ble_uuid.type = p_uart->uuid_type;
    ble_uuid.uuid = UART_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_uart->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add Write characteristic.  receive data from phone
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid                     = UART_UUID_WRITE_CHAR;
    add_char_params.uuid_type                = p_uart->uuid_type;
	
    add_char_params.max_len                  = BLE_UART_MAX_RX_CHAR_LEN;
    add_char_params.init_len                 = sizeof(uint8_t);
    add_char_params.is_var_len               = true;
    add_char_params.char_props.write         = 1;
    add_char_params.char_props.write_wo_resp = 1;

    add_char_params.read_access              = SEC_OPEN;
    add_char_params.write_access             = SEC_OPEN;

    err_code = characteristic_add(p_uart->service_handle,
                                  &add_char_params,
                                  &p_uart->rx_char_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add READ characteristic.
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = UART_UUID_READ_CHAR;
    add_char_params.uuid_type         = p_uart->uuid_type;
    add_char_params.max_len           = BLE_UART_MAX_TX_CHAR_LEN;
    add_char_params.init_len          = sizeof(uint8_t);
    add_char_params.is_var_len        = true;
    add_char_params.char_props.notify = 1;

    add_char_params.read_access       = SEC_OPEN;
    add_char_params.write_access      = SEC_OPEN;
    add_char_params.cccd_write_access = SEC_OPEN;

    return characteristic_add(p_uart->service_handle, &add_char_params, &p_uart->tx_char_handles);
}


static void on_write(ble_uart_t * p_uart, ble_evt_t const * p_ble_evt)
{
	
//	  ret_code_t                    err_code;
	
	  ble_uart_evt_t                evt;
	
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
	
	  memset(&evt, 0, sizeof(ble_uart_evt_t));
  
    evt.p_uart       = p_uart;
	
	  if ((p_evt_write->handle == p_uart->tx_char_handles.cccd_handle) && (p_evt_write->len == 2))
    {
			      if (ble_srv_is_notification_enabled(p_evt_write->data))
            {
                evt.evt_type                          = BLE_UART_EVT_COMM_STARTED;
            }
            else
            {
                evt.evt_type                          = BLE_UART_EVT_COMM_STOPPED;
            }

            if (p_uart->ble_data_rx_handler != NULL)
            {
                p_uart->ble_data_rx_handler(&evt);
            }
		}
		
		else if ((p_evt_write->handle == p_uart->rx_char_handles.value_handle) && (p_uart->ble_data_rx_handler != NULL))
    {
        evt.evt_type                  = BLE_UART_EVT_RX_DATA;
			
        evt.params.rx_data.p_data = p_evt_write->data;
        evt.params.rx_data.length = p_evt_write->len;

        p_uart->ble_data_rx_handler(&evt);
    }
    else
    {
        // Do Nothing. This event is not relevant for this service.
    }
				
}

/**@brief Connect event handler.
 *
 * @param[in]   p_uart       Nordic UART Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_uart_t * p_uart, ble_evt_t const * p_ble_evt)
{
//    p_nus->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;


//    if(p_nus->connect_handler != NULL) {
//        p_nus->connect_handler();
//    }
}


/**@brief Disconnect event handler.
 *
 * @param[in]   p_uart       Nordic UART Service structure.
 * @param[in]   p_ble_evt    Event received from the BLE stack.
 */
static void on_disconnect(ble_uart_t * p_uart, ble_evt_t const * p_ble_evt)
{
//    UNUSED_PARAMETER(p_ble_evt);
//    p_nus->conn_handle = BLE_CONN_HANDLE_INVALID;
//    p_nus->is_notification_enabled = false;
//    bluetooth_l0_reset();
}



static void on_hvx_tx_complete(ble_uart_t * p_uart, ble_evt_t const * p_ble_evt)
{
    ret_code_t                  err_code;
	
    ble_uart_evt_t              evt;
	
	  if(p_uart->ble_tx_complete_handler != NULL)
		{
			  memset(&evt, 0, sizeof(ble_uart_evt_t));
        evt.evt_type        = BLE_UART_EVT_TX_RDY;
        evt.p_uart          = p_uart;
			  p_uart->ble_tx_complete_handler(&evt);
		}
		
}


void ble_uart_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_uart_t * p_uart = (ble_uart_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
			 case BLE_GAP_EVT_CONNECTED:
            on_connect(p_uart, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_uart, p_ble_evt);
            break;
				
        case BLE_GATTS_EVT_WRITE:
            on_write(p_uart, p_ble_evt);
            break;
				
        case BLE_GATTS_EVT_HVN_TX_COMPLETE:
            on_hvx_tx_complete(p_uart, p_ble_evt);
            break;
				
        default:
            // No implementation needed.
            break;
    }
}





