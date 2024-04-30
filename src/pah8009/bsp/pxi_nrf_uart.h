#pragma once


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


//============================================================================
// Definitions
//============================================================================
typedef enum
{
    PXI_NRF_UART_BUADRATE_1200,
    PXI_NRF_UART_BUADRATE_2400,
    PXI_NRF_UART_BUADRATE_4800,
    PXI_NRF_UART_BUADRATE_9600,
    PXI_NRF_UART_BUADRATE_14400,
    PXI_NRF_UART_BUADRATE_19200,
    PXI_NRF_UART_BUADRATE_28800,
    PXI_NRF_UART_BUADRATE_38400,
    PXI_NRF_UART_BUADRATE_57600,
    PXI_NRF_UART_BUADRATE_76800,
    PXI_NRF_UART_BUADRATE_115200,
    PXI_NRF_UART_BUADRATE_230400,
    PXI_NRF_UART_BUADRATE_250000,
    PXI_NRF_UART_BUADRATE_460800,
    PXI_NRF_UART_BUADRATE_921600,
    PXI_NRF_UART_BUADRATE_1000000,  // 1Mega
    
} pxi_nrf_uart_baudrate;


typedef struct
{
    pxi_nrf_uart_baudrate   buadrate;
    
} pxi_nrf_uart_setting;


//============================================================================
// Public functions
//============================================================================
bool pxi_nrf_uart_init(void);
void pxi_nrf_uart_deinit(void);

bool pxi_nrf_uart_transmit(const uint8_t *data, size_t tx_size);
bool pxi_nrf_uart_receive(uint8_t *data, size_t rx_size);
bool pxi_nrf_uart_receive_non_blocking(uint8_t *data, size_t rx_size);
