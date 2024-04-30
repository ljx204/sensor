// pah
#include "pah800x_bsp.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

//============================================================================
// Definition
//============================================================================
#define PAH800xx_I2C_SLAVE_ID 0x40
#define I2C_PPG_PORT_ID 1


//============================================================================
// Public function
//============================================================================
void pah_delay_ms(uint32_t ms)
{
    nrf_delay_ms(ms);
}

uint64_t pah_get_tick_count(void)
{
    return pxi_nrf_get_tick_count();
}

pah_ret pah_comm_i2c_write(uint8_t addr, const uint8_t *data, size_t write_size)
{
    //DEBUG_PRINT("W A:0x%02x,D:0x%02x\n",addr,*data);
    pxi_nrf_i2c_write(addr, data, write_size);

    return PAH_RET_SUCCESS;
}

pah_ret pah_comm_i2c_read(uint8_t addr, uint8_t *data, size_t read_size)
{
    pxi_nrf_i2c_read(addr, data, read_size);
    //DEBUG_PRINT("R A:0x%02x,D:0x%02x\n",addr,*data);
    return PAH_RET_SUCCESS;
}

void pah_print(
    int32_t                         fileline,
    const char                      *funcname,
    const char                      *filename,
    const char                      *format, ...)
{
    UNUSED_VAR(fileline);
    UNUSED_VAR(funcname);
    UNUSED_VAR(filename);

    char dest[400];
    va_list argptr;
    int char_num = 0;

    if (!format)
        return;

    va_start(argptr, format);
    char_num = vsprintf(dest, format, argptr);
    va_end(argptr);

//    //uart_printf("%s", dest);
		
	    dest[char_num] = '\n';
		
	    NRF_LOG_INFO("%s", dest);
		
		NRF_LOG_PROCESS();
	

//    //
//    pxi_nrf_uart_transmit((uint8_t*)dest, (uint16_t)(char_num + 1));
}

void ReadGSensor(float *x, float *y, float *z)
{
    *x = 0.11111111;
	  *y = 1.1;
	  *z = 1.4444;
}
