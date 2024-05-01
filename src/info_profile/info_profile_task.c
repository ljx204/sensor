
#include "info_profile.h"

#include "info_profile_task.h"

#include "app_error.h"

#include "nrf_log.h"

BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);

static void nus_data_handler(ble_nus_evt_t *p_evt)
{

    if (p_evt->type == BLE_NUS_EVT_RX_DATA)
    {
        uint32_t err_code;

        NRF_LOG_DEBUG("Received data from BLE NUS. Writing data on UART.");
        NRF_LOG_HEXDUMP_DEBUG(p_evt->params.rx_data.p_data, p_evt->params.rx_data.length);

        for (uint32_t i = 0; i < p_evt->params.rx_data.length; i++)
        {
            do
            {
                //                err_code = app_uart_put(p_evt->params.rx_data.p_data[i]);
                //                if ((err_code != NRF_SUCCESS) && (err_code != NRF_ERROR_BUSY))
                //                {
                //                    NRF_LOG_ERROR("Failed receiving NUS message. Error 0x%x. ", err_code);
                //                    APP_ERROR_CHECK(err_code);
                //                }
            } while (err_code == NRF_ERROR_BUSY);
        }
        //        if (p_evt->params.rx_data.p_data[p_evt->params.rx_data.length - 1] == '\r')
        //        {
        //            while (app_uart_put('\n') == NRF_ERROR_BUSY);
        //        }
    }
}

void app_info_service_init(void)
{
    uint32_t err_code;

    ble_nus_init_t nus_init;
    // Initialize NUS.
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;

    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}
