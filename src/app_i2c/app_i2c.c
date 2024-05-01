/*
   liujian

     ctd_liu@163.com
*/

#include "app_i2c.h"
#include "app_util_platform.h"
#include "app_error.h"

#include "stk832x.h"

#include "pxi_nrf_i2c.h"

#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_drv_twi.h"

#define APP_SCL_PIN 3
#define APP_SDA_PIN 1

/* Indicates if operation on TWI has ended. */
static volatile bool m_xfer_done = false;

/* TWI instance ID. */
#define TWI_INSTANCE_ID 0

/* TWI instance. */
static const nrf_drv_twi_t m_app_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

void app_twi_init(void)
{
    ret_code_t err_code;

    const nrf_drv_twi_config_t app_twi_config = {
        .scl = APP_SCL_PIN,
        .sda = APP_SDA_PIN,
        .frequency = NRF_DRV_TWI_FREQ_400K,
        .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
        .clear_bus_init = false};

    err_code = nrf_drv_twi_init(&m_app_twi, &app_twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_app_twi);
}

void app_twi_uninit(void)
{
}

/**
 * @brief Function for STK832x register setting.
 */
void stk832x_register_set(uint8_t reg_id, uint8_t val)
{
#if defined(STK832x_CRASH_FUNC_ENABLE)
    ret_code_t err_code;
#endif /* defined(STK832x_CRASH_FUNC_ENABLE) */

    uint8_t reg[2] = {0};

    reg[0] = reg_id;
    reg[1] = val;

#if defined(STK832x_CRASH_FUNC_ENABLE)
    err_code = nrf_drv_twi_tx(&m_stk832x_twi, STK832x_ADDR, reg, 2, false);
    APP_ERROR_CHECK(err_code);
#else /* defined(STK832x_CRASH_FUNC_ENABLE) */

    nrf_drv_twi_tx(&m_app_twi, STK832x_ADDR, reg, 2, false);

#endif /* defined(STK832x_CRASH_FUNC_ENABLE) */
}

/**
 * @brief Function for STK832x register get.
 */

void stk832x_register_get(uint8_t reg_id, uint8_t *val)
{
#if defined(STK832x_CRASH_FUNC_ENABLE)
    ret_code_t err_code;
#endif /* defined(STK832x_CRASH_FUNC_ENABLE) */

    uint8_t m_value;
    uint8_t reg[2] = {0};

    reg[0] = reg_id;

#if defined(STK832x_CRASH_FUNC_ENABLE)
    err_code = nrf_drv_twi_tx(&m_stk832x_twi, STK832x_ADDR, &reg[0], 1, false);
    APP_ERROR_CHECK(err_code);
    /* Read 1 byte from the specified address to get Chip ID */
    err_code = nrf_drv_twi_rx(&m_stk832x_twi, STK832x_ADDR, &m_value, sizeof(m_value));
    APP_ERROR_CHECK(err_code);
    *val = m_value;
#else /* defined(STK832x_CRASH_FUNC_ENABLE) */

    nrf_drv_twi_tx(&m_app_twi, STK832x_ADDR, &reg[0], 1, false);

    /* Read 1 byte from the specified address to get Chip ID */
    nrf_drv_twi_rx(&m_app_twi, STK832x_ADDR, &m_value, sizeof(m_value));

    *val = m_value;

#endif /* defined(STK832x_CRASH_FUNC_ENABLE) */
}

/**
 * @brief Function for STK832x register get.
 */
void stk832x_registers_get(uint8_t reg_id, uint8_t *val, uint8_t size)
{
#if defined(STK832x_CRASH_FUNC_ENABLE)
    ret_code_t err_code;
#endif /* defined(STK832x_CRASH_FUNC_ENABLE) */

#if defined(STK832x_CRASH_FUNC_ENABLE)
    err_code = nrf_drv_twi_tx(&m_stk832x_twi, STK832x_ADDR, &reg_id, 1, false);
    APP_ERROR_CHECK(err_code);
    /* Read 1 byte from the specified address to get Chip ID */
    err_code = nrf_drv_twi_rx(&m_stk832x_twi, STK832x_ADDR, val, size);
    APP_ERROR_CHECK(err_code);
#else /* defined(STK832x_CRASH_FUNC_ENABLE) */

    nrf_drv_twi_tx(&m_app_twi, STK832x_ADDR, &reg_id, 1, false);

    /* Read 1 byte from the specified address to get Chip ID */
    nrf_drv_twi_rx(&m_app_twi, STK832x_ADDR, val, size);

#endif /* defined(STK832x_CRASH_FUNC_ENABLE) */
}

ret_code_t pxi_nrf_i2c_write(uint8_t addr, const uint8_t *data, size_t write_size)
{
    static uint8_t write_buffer[PXI_NRF_I2C_MAX_COMM_SIZE];

    if (write_size > sizeof(write_buffer))
        return NRF_ERROR_INVALID_LENGTH;

    write_buffer[0] = addr;
    memcpy(&write_buffer[1], data, write_size);

    //    app_twi_transfer_t const cfg_transfers[] = {
    //        APP_TWI_WRITE(i2c.config.slave_addr, write_buffer, write_size + 1, 0),
    //    };

    return nrf_drv_twi_tx(&m_app_twi, SLAVE_ID, write_buffer, write_size + 1, false);

    //   return app_twi_perform(&m_app_twi, cfg_transfers, ARRAY_SIZE(cfg_transfers), NULL);
}

ret_code_t pxi_nrf_i2c_read(uint8_t addr, uint8_t *data, size_t read_size)
{
    //    if (!m_has_init)
    //        return NRF_ERROR_INVALID_STATE;
    if (read_size > PXI_NRF_I2C_MAX_COMM_SIZE)
        return NRF_ERROR_INVALID_LENGTH;

    //    app_twi_transfer_t const cfg_transfers[] = {
    //        APP_TWI_WRITE(i2c.config.slave_addr, &addr, 1, 0),
    //        APP_TWI_READ(i2c.config.slave_addr, data, read_size, 0),
    //    };
    //
    //    return app_twi_perform(&m_app_twi, cfg_transfers, ARRAY_SIZE(cfg_transfers), NULL);

    nrf_drv_twi_tx(&m_app_twi, SLAVE_ID, &addr, 1, false);

    /* Read 1 byte from the specified address to get Chip ID */
    return nrf_drv_twi_rx(&m_app_twi, SLAVE_ID, data, sizeof(data));

    //    *val = m_value;
}

signed char stk832x_registers_set(unsigned char regAddr, unsigned char *regValue, unsigned char count)
{
    static uint8_t write_buffer[128];

    if (count > sizeof(write_buffer))
        return NRF_ERROR_INVALID_LENGTH;

    write_buffer[0] = regAddr;
    memcpy(&write_buffer[1], regValue, count);

    return nrf_drv_twi_tx(&m_app_twi, SLAVE_ID, write_buffer, count + 1, false);
}
