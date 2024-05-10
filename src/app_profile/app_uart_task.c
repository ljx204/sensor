/*
     Liujian

         2024/4/23

*/

#include "app_uart_task.h"

#include "nrf_log.h"

BLE_UART_DEF(m_uart);

extern uint16_t m_conn_handle;

static bool is_notification_enabled;

static SendCompletePara m_send_complete_para = {NULL, NULL, TASK_NONE};

bool app_uart_send_ready(void)
{
    if (m_conn_handle == BLE_CONN_HANDLE_INVALID)
        return false;

    if (is_notification_enabled == false)
        return false;

    return true;
}

// ��ɷ��ͺ�Ļص�
void set_complete_callback(SendCompletePara para)
{
    m_send_complete_para.callback = para.callback;

    m_send_complete_para.context = para.context;

    m_send_complete_para.task_type = para.task_type;
}

static void app_uart_rx_data_handler(ble_uart_evt_t *p_evt)
{
    if (p_evt->evt_type == BLE_UART_EVT_RX_DATA)
    {

       // NRF_LOG_INFO("Received %d", p_evt->params.rx_data.length);
			 for(int i = 0; i < p_evt->params.rx_data.length; i++)
			 {
				   NRF_LOG_INFO("0x%02x ", p_evt->params.rx_data.p_data[i]);
			 }

        L1_receive_data((uint8_t *)p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);
    }
    else if (p_evt->evt_type == BLE_UART_EVT_COMM_STARTED)
    {
        NRF_LOG_INFO("EANBLE");
        is_notification_enabled = true;
    }

    else if (p_evt->evt_type == BLE_UART_EVT_COMM_STOPPED)
    {
        NRF_LOG_INFO("STOPPED");
        is_notification_enabled = false;
    }
}

static void app_uart_tx_complete_data_handler(ble_uart_evt_t *p_evt)
{
    if (m_send_complete_para.callback)
    {
        m_send_complete_para.callback(m_send_complete_para.context, m_send_complete_para.task_type);
    }
}

void app_uart_service_init(void)

{
    ret_code_t err_code;

    ble_uart_init_t init = {0};

    // Initialize LBS.
    m_uart.uuid_type = BLE_UUID_TYPE_BLE;

    init.uart_data_handler = app_uart_rx_data_handler;
    init.uart_tx_complete_handler = app_uart_tx_complete_data_handler;

    err_code = ble_uart_init(&m_uart, &init);
    APP_ERROR_CHECK(err_code);
}

uint32_t app_uart_data_send(ble_uart_t *p_uart, uint8_t *p_data, uint16_t *p_length, uint16_t conn_handle)
{
    //    ret_code_t                 err_code;
    ble_gatts_hvx_params_t hvx_params;

    VERIFY_PARAM_NOT_NULL(p_uart);

    if (conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_NOT_FOUND;
    }

    if (!is_notification_enabled)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (*p_length > BLE_UART_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_uart->tx_char_handles.value_handle;
    hvx_params.p_data = p_data;
    hvx_params.p_len = p_length;
    hvx_params.type = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(conn_handle, &hvx_params);
}

uint32_t ble_uart_send_string(uint8_t *string, uint16_t *length)
{
    ble_gatts_hvx_params_t hvx_params;

    //    if (!p_nus->is_notification_enabled) {
    //        return NRF_ERROR_INVALID_STATE;
    //    }

    //    if (*length > NUS_MAX_DATA_LENGTH) {
    //        return NRF_ERROR_INVALID_PARAM;
    //    }

#ifdef DEBUG_LOG
    uint8_t dumpContent[41];
    uint32_t i = 0;
    for (; i < (*length) * 2; i += 2)
    {
        sprintf(((char *)dumpContent) + i, "%.2x", string[i / 2]);
    }
    dumpContent[(*length) * 2] = 0; // string end

    if (string[0] == 0xAB && string[1] == 0x10)
    {
        LOG(LEVEL_INFO, "res pack & con is:%s\r\n", dumpContent);
    }
    else
    {
        LOG(LEVEL_INFO, "data pack & con is:%s\r\n", dumpContent);
    }
#endif

    memset(&hvx_params, 0, sizeof(hvx_params));
    hvx_params.handle = m_uart.tx_char_handles.value_handle;
    hvx_params.p_data = string;
    hvx_params.p_len = length;
    hvx_params.type = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(m_conn_handle, &hvx_params);
}

static uint8_t test_var = 0;
void app_button_send_data(void)
{
    uint8_t test_data[250];

    uint16_t length = 244;

    for (int i = 0; i < 250; i++)
    {
        test_data[i] = test_var;
    }

    test_var++;

    app_uart_data_send(&m_uart, test_data, &length, m_conn_handle);

    NRF_LOG_INFO("length = %d", length);
}

// ����, �ı�������ַ
void app_change_ble_addr(void)
{
    ret_code_t err_code;
    ble_gap_addr_t addr;
    err_code = sd_ble_gap_addr_get(&addr);
    APP_ERROR_CHECK(err_code);

    // Increase the BLE address by one when advertising openly.
    addr.addr[0] += 17;

    err_code = sd_ble_gap_addr_set(&addr);
    APP_ERROR_CHECK(err_code);
}
