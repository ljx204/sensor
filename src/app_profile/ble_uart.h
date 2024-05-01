/*
    liujian define

        2024/4/23

*/

#ifndef __BLE_UART_H__
#define __BLE_UART_H__
#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define BLE_UART_BLE_OBSERVER_PRIO 2

#define UART_UUID_SERVICE 0xFF00
#define UART_UUID_WRITE_CHAR 0xFF01
#define UART_UUID_READ_CHAR 0xFF02
#define UART_UUID_BLE_CHAR 0xFF03 // �޸Ĺ㲥

#define OPCODE_LENGTH 1
#define HANDLE_LENGTH 2

/**@brief   Maximum length of data (in bytes) that can be transmitted to the peer by the UART service module. */
#if defined(NRF_SDH_BLE_GATT_MAX_MTU_SIZE) && (NRF_SDH_BLE_GATT_MAX_MTU_SIZE != 0)
#define BLE_UART_MAX_DATA_LEN (NRF_SDH_BLE_GATT_MAX_MTU_SIZE - OPCODE_LENGTH - HANDLE_LENGTH)
#else
#define BLE_UART_MAX_DATA_LEN (BLE_GATT_MTU_SIZE_DEFAULT - OPCODE_LENGTH - HANDLE_LENGTH)
#warning NRF_SDH_BLE_GATT_MAX_MTU_SIZE is not defined.
#endif

#define BLE_UART_DEF(_name)                          \
    static ble_uart_t _name;                         \
    NRF_SDH_BLE_OBSERVER(_name##_obs,                \
                         BLE_UART_BLE_OBSERVER_PRIO, \
                         ble_uart_on_ble_evt, &_name)

    typedef struct ble_uart_s ble_uart_t;

    typedef enum
    {
        BLE_UART_EVT_RX_DATA, /**< Data received. */

        BLE_UART_EVT_TX_RDY, /**< Service is ready to accept new data to be transmitted. */

        BLE_UART_EVT_COMM_STARTED, /**< Notification has been enabled. */

        BLE_UART_EVT_COMM_STOPPED, /**< Notification has been disabled. */

    } ble_uart_evt_type_t;

    typedef struct
    {
        uint8_t const *p_data; /**< A pointer to the buffer with received data. */

        uint16_t length; /**< Length of received data. */

    } ble_uart_evt_rx_data_t;

    typedef struct
    {
        ble_uart_evt_type_t evt_type; /**< Type of event. */

        ble_uart_t *p_uart; /**< A pointer to the instance. */

        union
        {
            ble_uart_evt_rx_data_t rx_data; /**< @ref BLE_UART_EVT_RX_DATA event data. */

        } params;

    } ble_uart_evt_t;

    typedef void (*ble_uart_rx_handler_t)(ble_uart_evt_t *p_evt);

    typedef void (*ble_uart_tx_complete_handler_t)(ble_uart_evt_t *p_evt);

    typedef struct
    {
        /**< Event handler to be called when the write Characteristic is written. */

        ble_uart_rx_handler_t uart_data_handler;

        ble_uart_tx_complete_handler_t uart_tx_complete_handler;

    } ble_uart_init_t;

    /**@brief UART Service structure. This structure contains various status information for the service. */

    struct ble_uart_s
    {
        uint16_t service_handle; /**< Handle of UART Service (as provided by the BLE stack). */

        ble_gatts_char_handles_t rx_char_handles; /**< Handles related to the write Characteristic. */

        ble_gatts_char_handles_t tx_char_handles; /**< Handles related to the read Characteristic. */

        ble_gatts_char_handles_t ble_char_handles; /**< Handles related to the ble Characteristic. */

        uint8_t uuid_type; /**< UUID type for the UART Service. */

        ble_uart_rx_handler_t ble_data_rx_handler; /**< Event handler to be called when the write Characteristic is written. */

        ble_uart_tx_complete_handler_t ble_tx_complete_handler;
    };

    uint32_t ble_uart_init(ble_uart_t *p_lbs, const ble_uart_init_t *p_uart_init);

    void ble_uart_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context);

    // uint32_t ble_lbs_on_button_change(uint16_t conn_handle, ble_lbs_t * p_lbs, uint8_t button_state);

    // uint32_t ble_uart_data_send(ble_uart_t * p_nus, uint8_t   * p_data, uint16_t  * p_length, uint16_t    conn_handle);

#ifdef __cplusplus
}
#endif

#endif
