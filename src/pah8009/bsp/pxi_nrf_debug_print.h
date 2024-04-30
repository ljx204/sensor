#pragma once


#include "pxi_nrf_uart.h"

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>


//============================================================================
// Option
//============================================================================
#define ENABLE_UART_LOG


//============================================================================
// Public functions
//============================================================================
#ifdef ENABLE_UART_LOG
    static char _uart_buffer[128];
    static void _uart_printf(const char *format, ...)
    {
        va_list args;
        int char_num = 0;
        
        if (!format)
            return;
    
        va_start(args, format);
        //char_num = vsiprintf(uart_buffer, format, args);
        //char_num = _vsprintf(uart_buffer, format, args);
        char_num = vsprintf(_uart_buffer, format, args);
        va_end(args);
        
        pxi_nrf_uart_transmit((uint8_t*)_uart_buffer, (uint16_t)char_num);
    }
#endif

#ifdef ENABLE_UART_LOG
#   define DEBUG_PRINT(...) \
    do { \
        _uart_printf(__VA_ARGS__); \
    } while (0)
#else
#   define DEBUG_PRINT(...) \
    do { \
    } while (0)
#endif

