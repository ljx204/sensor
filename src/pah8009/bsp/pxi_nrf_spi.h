#pragma once


// bsp
#include <nrf_drv_spi.h>


//============================================================================
// Definitions
//============================================================================
#define PXI_NRF_SPI_MAX_COMM_SIZE   (254)


typedef enum
{
    PXI_NRF_SPI_FREQUENCY_125K = 125,   // 125 kbps
    PXI_NRF_SPI_FREQUENCY_250K = 250,   // 250 kbps
    PXI_NRF_SPI_FREQUENCY_500K = 500,   // 500 kbps
    PXI_NRF_SPI_FREQUENCY_1M = 1000,    // 1 Mbps
    PXI_NRF_SPI_FREQUENCY_2M = 2000,    // 2 Mbps
    PXI_NRF_SPI_FREQUENCY_4M = 4000,    // 4 Mbps
    PXI_NRF_SPI_FREQUENCY_8M = 8000,    // 8 Mbps
    
} pxi_nrf_spi_frequency;


typedef enum
{
    // CSK(25), MISO(24), MOSI(23), CSN(22)
    PXI_NRF_SPI_INSTANCE_1,
    
    // CSK(29), MISO(30), MOSI(31), CSN(28)
    //PXI_NRF_SPI_INSTANCE_2,
    
} pxi_nrf_spi_instance;
    

typedef struct
{
    pxi_nrf_spi_frequency   frequency;
    pxi_nrf_spi_instance    instance;
    
} pxi_nrf_spi_setting;


typedef struct
{
    const nrf_drv_spi_t   *drv_spi;
    
} pxi_nrf_spi;


//============================================================================
// Public functions
//============================================================================
ret_code_t  pxi_nrf_spi_init(const pxi_nrf_spi_setting *spi_setting);
void        pxi_nrf_spi_deinit(void);

bool        pxi_nrf_spi_register(pxi_nrf_spi *spi);

ret_code_t  pxi_nrf_spi_write(pxi_nrf_spi *spi, uint8_t addr, const uint8_t *data, size_t write_size);
ret_code_t  pxi_nrf_spi_read(pxi_nrf_spi *spi, uint8_t addr, uint8_t *data, size_t read_size);

