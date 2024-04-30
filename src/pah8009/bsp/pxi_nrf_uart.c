/*============================================================================
* To enable UART
*
* 1. Open "nrf_drv_config.h"
*  // UART
*  #define UART0_ENABLED 1
*  
*  #if (UART0_ENABLED == 1)
*  #define UART0_CONFIG_HWFC         NRF_UART_HWFC_DISABLED
*  #define UART0_CONFIG_PARITY       NRF_UART_PARITY_EXCLUDED
*  #define UART0_CONFIG_BAUDRATE     NRF_UART_BAUDRATE_115200
*  #define UART0_CONFIG_PSEL_TXD 6
*  #define UART0_CONFIG_PSEL_RXD 8
*  #define UART0_CONFIG_PSEL_CTS 7
*  #define UART0_CONFIG_PSEL_RTS 5
*  #define UART0_CONFIG_IRQ_PRIORITY APP_IRQ_PRIORITY_LOW
*  #ifdef NRF52
*  #define UART0_CONFIG_USE_EASY_DMA false
*  //Compile time flag
*  #define UART_EASY_DMA_SUPPORT     1
*  #define UART_LEGACY_SUPPORT       1
*  #endif //NRF52
*  #endif
*
* 2. Add header search path
*  \components\drivers_nrf\uart
*  \components\libraries\fifo
*  \components\libraries\uart
*
* 3. Add bsp source files
*  \components\drivers_nrf\uart\nrf_drv_uart.c
*  \components\libraries\fifo\app_fifo.c
*  \components\libraries\uart\app_uart_fifo.c
*
============================================================================*/

#include "pxi_nrf_uart.h"

// bsp
#include <bsp.h>
#include <app_uart.h>

// c
#include <string.h>


//============================================================================
// Definitions
//============================================================================
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */


//============================================================================
// Private variables
//============================================================================
static size_t   rx_buffer_len = 0;

    
//============================================================================
// Private functions
//============================================================================
static void _uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}


//============================================================================
// Public functions
//============================================================================
bool pxi_nrf_uart_init(void)
{
	pxi_nrf_uart_setting    uart_setting = {
        .buadrate   = PXI_NRF_UART_BUADRATE_115200,
    };

    ret_code_t err_code = NRF_SUCCESS;
    
    pxi_nrf_uart_deinit();
    
    rx_buffer_len = 0;
    
    uint32_t baud_rate = UART_BAUDRATE_BAUDRATE_Baud115200;
    
    switch (uart_setting.buadrate)
    {
        case PXI_NRF_UART_BUADRATE_1200:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud1200;
            break;
        case PXI_NRF_UART_BUADRATE_2400:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud2400;
            break;
        case PXI_NRF_UART_BUADRATE_4800:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud4800;
            break;
        case PXI_NRF_UART_BUADRATE_9600:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud9600;
            break;
        case PXI_NRF_UART_BUADRATE_14400:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud14400;
            break;
        case PXI_NRF_UART_BUADRATE_19200:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud19200;
            break;
        case PXI_NRF_UART_BUADRATE_28800:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud28800;
            break;
        case PXI_NRF_UART_BUADRATE_38400:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud38400;
            break;
        case PXI_NRF_UART_BUADRATE_57600:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud57600;
            break;
        case PXI_NRF_UART_BUADRATE_76800:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud76800;
            break;
        case PXI_NRF_UART_BUADRATE_115200:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud115200;
            break;
        case PXI_NRF_UART_BUADRATE_230400:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud230400;
            break;
        case PXI_NRF_UART_BUADRATE_250000:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud250000;
            break;
        case PXI_NRF_UART_BUADRATE_460800:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud460800;
            break;
        case PXI_NRF_UART_BUADRATE_921600:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud921600;
            break;
        case PXI_NRF_UART_BUADRATE_1000000:
            baud_rate = UART_BAUDRATE_BAUDRATE_Baud1M;
            break;
        default:
            break;
    }
    
    const app_uart_comm_params_t comm_params =
    {
        .rx_pin_no      = RX_PIN_NUMBER,
        .tx_pin_no      = TX_PIN_NUMBER,
        .rts_pin_no     = RTS_PIN_NUMBER,
        .cts_pin_no     = CTS_PIN_NUMBER,
        .flow_control   = APP_UART_FLOW_CONTROL_ENABLED,
        .use_parity     = false,
        .baud_rate      = baud_rate,
    };
    
    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       _uart_error_handle,
                       APP_IRQ_PRIORITY_LOW,
                       err_code);
    
    if (err_code != NRF_SUCCESS)
        return false;
    
    return true;
}

void pxi_nrf_uart_deinit(void)
{

}

bool pxi_nrf_uart_transmit(const uint8_t *data, size_t tx_size)
{
    uint32_t i = 0;
    
    for (i = 0; i < tx_size; i++)
    {
        while (app_uart_put(data[i]) != NRF_SUCCESS);
    }
    
    return true;
}

bool pxi_nrf_uart_receive(uint8_t *data, size_t rx_size)
{
    size_t i = 0;
    
    for (i = 0; i < rx_size; i++)
    {
        while (app_uart_get(&data[i]) != NRF_SUCCESS);
    }
    
    return true;
}

bool pxi_nrf_uart_receive_non_blocking(uint8_t *data, size_t rx_size)
{
    while (rx_buffer_len < rx_size)
    {
        if (app_uart_get(&data[rx_buffer_len]) != NRF_SUCCESS)
            break;
        ++rx_buffer_len;
    }
    
    if (rx_buffer_len < rx_size)
        return false;
 
    rx_buffer_len = 0;
    return true;
}
