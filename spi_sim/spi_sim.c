#include "spi_sim.h"

#define SPI_SIM_CS(pin, x)
#define SPI_SIM_CLK(pin, x)
#define SPI_SIM_MOSI(pin, x)
#define SPI_SIM_MISO(pin)            0
#define SPI_SIM_DELAY()

static spi_sim_control g_spi_sim_control[SPI_SIM_NUM];

static uint8_t spi_sim_clock_output(spi_sim_param *param, uint8_t data, uint8_t re_wr)
{
    uint8_t re_data = 0;
    if (param->cpol == 1) {
        SPI_SIM_CLK(param->clk_pin, 1);
    } else {
        SPI_SIM_CLK(param->clk_pin, 0);
    }

    if (re_wr == SPI_SIM_WRITE || re_wr == SPI_SIM_WRITE_AND_READ) {
        SPI_SIM_MOSI(param->mosi_pin, data);
    }
    SPI_SIM_DELAY();

    if (param->cpol == 1) {
        SPI_SIM_CLK(param->clk_pin, 0);
    } else {
        SPI_SIM_CLK(param->clk_pin, 1);
    }
    SPI_SIM_DELAY();

    if (param->cpha == 1) {
        if (param->cpol == 1) {
            SPI_SIM_CLK(param->clk_pin, 1);
        } else {
            SPI_SIM_CLK(param->clk_pin, 0);
        }
        SPI_SIM_DELAY();
    }

    re_data =  SPI_SIM_MISO(param->miso_pin);

    if (param->cpol == 1) {
        SPI_SIM_CLK(param->clk_pin, 1);
    } else {
        SPI_SIM_CLK(param->clk_pin, 0);
    }
}

void spi_sim_write_byte(uint8_t serial, uint8_t data) 
{
    uint8_t temp = 0;
    if (serial >= SPI_SIM_NUM) {
        return ;
    }

    SPI_SIM_CS(g_spi_sim_control[serial].param.cs_pin, 0);
    for (uint8_t i = 0; i < 8; i++) {
        temp = ((data & 0x80) == 0x80) ? 1 : 0;
        data = data << 1;
        spi_sim_clock_output(&g_spi_sim_control[serial].param, temp, SPI_SIM_WRITE);
    }
    SPI_SIM_CS(g_spi_sim_control[serial].param.cs_pin, 1);
}

uint8_t spi_sim_read_byte(uint8_t serial)
{
    uint8_t read_data = 0xFF;
    if (serial >= SPI_SIM_NUM) {
        return 0xFF;
    }

    SPI_SIM_CS(g_spi_sim_control[serial].param.cs_pin, 0);
    for (uint8_t i = 0; i < 8; i++) {
        read_data = read_data << 1;
        if (spi_sim_clock_output(0xFF, SPI_SIM_READ) == 1) {
            read_data += 1;
        }
    }
    SPI_SIM_CS(g_spi_sim_control[serial].param.cs_pin, 1);

    return read_data;
}

uint8_t spi_sim_write_read_byte(uint8_t serial, uint8_t data)
{
    uint8_t temp = 0;
    uint8_t read_data = 0xFF;
    if (serial >= SPI_SIM_NUM) {
        return 0xFF;
    }

    SPI_SIM_CS(g_spi_sim_control[serial].param.cs_pin, 0);
    for (uint8_t i = 0; i < 8; i++) {
        temp = ((data & 0x80) == 0x80) ? 1 : 0;
        read_data = read_data << 1;
        if (spi_sim_clock_output(temp, SPI_SIM_WRITE_AND_READ) == 1) {
            read_data += 1;
        }
    }
    SPI_SIM_CS(g_spi_sim_control[serial].param.cs_pin, 1);

    return read_data;
}

uint8_t spi_sim_register(spi_sim_param *param)
{
    uint8_t i = 0;
    for (i = 0; i < SPI_SIM_NUM; i++) {
        if (g_spi_sim_control[i].inited == false) {
            break;
        }
    }
    
    if (i >= SPI_SIM_NUM) {
        return 0xFF;
    }
    
    g_spi_sim_control[i].param.clk_pin = param->clk_pin;
    g_spi_sim_control[i].param.miso_pin = param->miso_pin;
    g_spi_sim_control[i].param.mosi_pin = param->mosi_pin;
    g_spi_sim_control[i].param.cs_pin = param->cs_pin;
    g_spi_sim_control[i].param.cpha = param->cpha;
    g_spi_sim_control[i].param.cpol = param->cpol;
    g_spi_sim_control[i].inited = true;
    
	spi_sim_ports_pin_init(param);
    return i;
}

void spi_sim_unregister(uint8_t serial)
{
    if (serial >= SPI_SIM_NUM) {
        return ;
    }

    g_spi_sim_control[serial].inited = false;
}