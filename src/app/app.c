/*
        liujian

        ctd_liu@163.com

        2024/4/24

*/
#include "app.h"
#include "app_i2c.h"
#include "bd_communicate_protocol.h"
#include "app_ble.h"
#include "app_pah8009.h"
#include "app_scheduler.h"
#include "app_saadc.h"
#include "nrf_pwr_mgmt.h"
#include "app_motion.h"
#include "app_process_data.h"

#include "app_uart.h"

#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_delay.h"

#define UART_TX_BUF_SIZE                256                                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE                256                                         /**< UART RX buffer size. */

/**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */
#define DEAD_BEEF 0xDEADBEEF

/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num   Line number of the failing ASSERT call.
 * @param[in] file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/**@brief Function for starting timers.
 */
static void application_timers_start(void)
{
    // sensor_timer_create();
}

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void timers_init(void)
{
    // Initialize timer module.
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

static void app_schduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

/**@brief  Function for initializing the UART module.
 */
/**@snippet [UART Initialization] */
static void uart_init(void)
{
    uint32_t                     err_code;
    app_uart_comm_params_t const comm_params =
    {
        .rx_pin_no    = RX_PIN_NUMBER,
        .tx_pin_no    = TX_PIN_NUMBER,
        .rts_pin_no   = RTS_PIN_NUMBER,
        .cts_pin_no   = CTS_PIN_NUMBER,
        .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
        .use_parity   = false,
#if defined (UART_PRESENT)
        .baud_rate    = NRF_UART_BAUDRATE_115200
#else
        .baud_rate    = NRF_UARTE_BAUDRATE_115200
#endif
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_event_handle,
                       APP_IRQ_PRIORITY_LOWEST,
                       err_code);
    APP_ERROR_CHECK(err_code);
}


void app_init(void)
{

    ret_code_t err_code;

    // Initialize.
    log_init();

#if PPG_SCHEDULER_ENABLE
    app_schduler_init();
#endif

    // ���� OTA ����
    //	  err_code = ble_dfu_buttonless_async_svci_init();
    //    APP_ERROR_CHECK(err_code);

    timers_init();

    power_management_init();

    app_ble_init();

    application_timers_start();

    //   app_change_ble_addr();

    app_twi_init();

    STK832x_stkMotion_init();

    //		while(1)
    //		{
    //
    //		    pah8009_set_led(LED_GREEN);
    //			  nrf_delay_ms(2000);
    //			  pah8009_led_off();
    //			  nrf_delay_ms(1000);
    //		}

    app_pah8009_process();

    // init bluetooth communicate protocol
    bluetooth_l0_init();

    app_saadc_init();

//    app_process_data_init();
}
