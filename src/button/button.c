/*
     Liujian

         2024/4/23

*/

#include "nordic_common.h"
#include "nrf.h"
#include "app_button.h"
#include "app_timer.h"
#include "app_uart_task.h"
#include "boards.h"
#include "ble_err.h"
#include "nrf_log.h"
#include "button.h"

#define TEST_BUTTON1 BSP_BUTTON_0
#define BUTTON_DETECTION_DELAY APP_TIMER_TICKS(50)

static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
    ret_code_t err_code;

    switch (pin_no)
    {
    case TEST_BUTTON1:
        NRF_LOG_INFO("Send button state change.");
        //            err_code = ble_lbs_on_button_change(m_conn_handle, &m_lbs, button_action);
        //            if (err_code != NRF_SUCCESS &&
        //                err_code != BLE_ERROR_INVALID_CONN_HANDLE &&
        //                err_code != NRF_ERROR_INVALID_STATE &&
        //                err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
        //            {
        //                APP_ERROR_CHECK(err_code);
        //            }

        app_button_send_data();

        break;

    default:
        APP_ERROR_HANDLER(pin_no);
        break;
    }
}

void sensor_buttons_init(void)
{
    ret_code_t err_code;

    // The array must be static because a pointer to it will be saved in the button handler module.
    static app_button_cfg_t buttons[] =
        {
            {TEST_BUTTON1, false, BUTTON_PULL, button_event_handler}};

    err_code = app_button_init(buttons, ARRAY_SIZE(buttons), BUTTON_DETECTION_DELAY);

    APP_ERROR_CHECK(err_code);
}
