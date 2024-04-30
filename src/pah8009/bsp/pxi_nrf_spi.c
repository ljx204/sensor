/*============================================================================
* To enable spi
*
* 1. Open "nrf_drv_config.h"
* #define SPI1_ENABLED 1
*
* 2. Add header search path
*  \components_pxi\drivers_nrf\spi_master
*
* 3. Add bsp source files
*  \components_pxi\drivers_nrf\spi_master\nrf_drv_spi.c
*
============================================================================*/

#include "pxi_nrf_spi.h"

#include <string.h>

// bsp
#include <nrf_drv_config.h>
#include <nrf_gpio.h>
#include <app_util_platform.h>


//============================================================================
// Macro
//============================================================================
#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof(arr[0]))


//============================================================================
// Definitions
//============================================================================
#define SPI_INSTANCE        1
#define SPI_MAX_USED_COUNT  (1)


#define SPI_1_CONFIG_SCK_PIN          (25)
#define SPI_1_CONFIG_MISO_PIN         (24)
#define SPI_1_CONFIG_MOSI_PIN         (23)
#define SPI_1_CSN_PIN                 (22)

#define SPI_2_CSN_PIN                 (28)
#define SPI_2_CONFIG_SCK_PIN          (29)
#define SPI_2_CONFIG_MOSI_PIN         (30)
#define SPI_2_CONFIG_MISO_PIN         (31)


//============================================================================
// Private global variables
//============================================================================
static const nrf_drv_spi_t  _spi_instance_1 = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */


//============================================================================
// Public functions
//============================================================================
ret_code_t pxi_nrf_spi_init(const pxi_nrf_spi_setting *spi_setting)
{
    ret_code_t err_code = 0;
    
    if (!spi_setting)
        return NRF_ERROR_INVALID_PARAM;
    
    // determine instance
    uint8_t sck_pin = SPI_1_CONFIG_SCK_PIN;
    uint8_t mosi_pin = SPI_1_CONFIG_MOSI_PIN;
    uint8_t miso_pin = SPI_1_CONFIG_MISO_PIN;
    uint8_t ss_pin = SPI_1_CSN_PIN;
    
    nrf_drv_spi_frequency_t frequency = NRF_DRV_SPI_FREQ_8M;
    switch (spi_setting->frequency)
    {
        case PXI_NRF_SPI_FREQUENCY_125K:
            frequency = NRF_DRV_SPI_FREQ_125K;
            break;
        case PXI_NRF_SPI_FREQUENCY_250K:
            frequency = NRF_DRV_SPI_FREQ_250K;
            break;
        case PXI_NRF_SPI_FREQUENCY_500K:
            frequency = NRF_DRV_SPI_FREQ_500K;
            break;
        case PXI_NRF_SPI_FREQUENCY_1M:
            frequency = NRF_DRV_SPI_FREQ_1M;
            break;
        case PXI_NRF_SPI_FREQUENCY_2M:
            frequency = NRF_DRV_SPI_FREQ_2M;
            break;
        case PXI_NRF_SPI_FREQUENCY_4M:
            frequency = NRF_DRV_SPI_FREQ_4M;
            break;
        case PXI_NRF_SPI_FREQUENCY_8M:
            frequency = NRF_DRV_SPI_FREQ_8M;
            break;
        default:
            return NRF_ERROR_INVALID_PARAM;
    }
    
    const nrf_drv_spi_config_t nrf_drv_spi_config = {
        .sck_pin      = sck_pin,
        .mosi_pin     = mosi_pin,
        .miso_pin     = miso_pin,
        .ss_pin       = ss_pin,
        .irq_priority = SPI1_CONFIG_IRQ_PRIORITY,
        .orc          = 0xFF,
        .frequency    = frequency,    // NRF_DRV_SPI_FREQ_125K, NRF_DRV_SPI_FREQ_250K, NRF_DRV_SPI_FREQ_500K, ..., NRF_DRV_SPI_FREQ_4M
        .mode         = NRF_DRV_SPI_MODE_3,
        .bit_order    = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST,
    };
    
    err_code = nrf_drv_spi_init(&_spi_instance_1, &nrf_drv_spi_config, NULL);
    if (err_code != NRF_SUCCESS)
        return err_code;
    
    return NRF_SUCCESS;
}

void pxi_nrf_spi_deinit(void)
{
    nrf_drv_spi_uninit(&_spi_instance_1);
}

bool pxi_nrf_spi_register(pxi_nrf_spi *spi)
{
    if (!spi)
        return false;

    memset(spi, 0, sizeof(*spi));

    spi->drv_spi = &_spi_instance_1;
    
    return true;
}

void pxi_nrf_spi_deregister(pxi_nrf_spi *spi)
{
    if (!spi)
        return;
    if (!spi->drv_spi)
        return;
    
    nrf_drv_spi_uninit(spi->drv_spi);
    memset(spi, 0, sizeof(*spi));
}

ret_code_t pxi_nrf_spi_write(pxi_nrf_spi *spi, uint8_t addr, const uint8_t *data, size_t write_size)
{
    static uint8_t write_buffer[PXI_NRF_SPI_MAX_COMM_SIZE];
    
    if (write_size > sizeof(write_buffer))
        return NRF_ERROR_INVALID_LENGTH;
    
    write_buffer[0] = addr;
    memcpy(&write_buffer[1], data, write_size);
    
    return nrf_drv_spi_transfer(spi->drv_spi, write_buffer, write_size + 1, NULL, 0);
}

ret_code_t pxi_nrf_spi_read(pxi_nrf_spi *spi, uint8_t addr, uint8_t *data, size_t read_size)
{
    ret_code_t err_code = 0;
    static const uint8_t WRITE_LENGTH = 1;
    static uint8_t buffer[PXI_NRF_SPI_MAX_COMM_SIZE];
    
    memset(buffer, 0, sizeof(buffer));
    
    if (read_size > sizeof(buffer) - 1)
        return NRF_ERROR_INVALID_LENGTH;
    
    buffer[0] = addr;
    
    err_code = nrf_drv_spi_transfer(spi->drv_spi, buffer, WRITE_LENGTH, buffer, WRITE_LENGTH + read_size);
    if (err_code != NRF_SUCCESS)
        return err_code;
    
    memcpy(data, &buffer[WRITE_LENGTH], read_size);
    
    return NRF_SUCCESS;
}


