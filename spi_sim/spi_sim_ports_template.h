#ifndef __SPI_SIM_PORTS_TEMPLATE_H__
#define __SPI_SIM_PORTS_TEMPLATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

#include "spi_sim.h"

#define SPI_SIM_NUM                5

void spi_sim_ports_cs_pin_set(uint8_t pin, uint8_t data);

void spi_sim_ports_clk_pin_set(uint8_t pin, uint8_t data);

void spi_sim_ports_mosi_pin_set(uint8_t pin, uint8_t data);

uint8_t spi_sim_ports_miso_pin_get(uint8_t pin);

void spi_sim_ports_pin_init(spi_sim_param *param);

#ifdef __cplusplus
}
#endif

#endif 
