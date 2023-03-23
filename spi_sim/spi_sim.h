#ifndef __SPI_SIM_H__
#define __SPI_SIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "spi_sim_ports.h"

enum {
    SPI_SIM_WRITE,
    SPI_SIM_READ,
    SPI_SIM_WRITE_AND_READ,
};

typedef struct _spi_sim_param {
    uint8_t cs_pin;
    uint8_t clk_pin;
    uint8_t mosi_pin;
    uint8_t miso_pin;
    bool cpol;
    bool cpha;
} spi_sim_param;

typedef struct _spi_sim_control{
    uint8_t inited;
    spi_sim_param param;
} spi_sim_control;

void spi_sim_write_byte(uint8_t serial, uint8_t data);

uint8_t spi_sim_read_byte(uint8_t serial);

uint8_t spi_sim_write_read_byte(uint8_t serial, uint8_t data);

uint8_t spi_sim_register(spi_sim_param *param);

void spi_sim_unregister(uint8_t serial);

#ifdef __cplusplus
}
#endif

#endif 
