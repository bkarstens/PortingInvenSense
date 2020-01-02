/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_usart_base.h>

struct spi_m_sync_descriptor SPI_0;

struct usart_sync_descriptor USART_0;

void USART_0_PORT_init(void)
{

	gpio_set_pin_function(PB2, MUX_PB2A_FLEXCOM1_RXD);

	gpio_set_pin_function(PB3, MUX_PB3A_FLEXCOM1_TXD);
}

void USART_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_FLEXCOM1);
}

void USART_0_init(void)
{
	USART_0_CLOCK_init();
	USART_0_PORT_init();
	usart_sync_init(&USART_0, FLEXCOM1, _usart_get_usart_sync());
}

void SPI_0_PORT_init(void)
{

	gpio_set_pin_function(PA12, MUX_PA12A_FLEXCOM5_MISO);

	gpio_set_pin_function(PA13, MUX_PA13A_FLEXCOM5_MOSI);

	gpio_set_pin_function(PA14, MUX_PA14A_FLEXCOM5_SPCK);
}

void SPI_0_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_FLEXCOM5);
}

void SPI_0_init(void)
{
	SPI_0_CLOCK_init();
	spi_m_sync_set_func_ptr(&SPI_0, NULL);
	spi_m_sync_init(&SPI_0, FLEXCOM5);
	SPI_0_PORT_init();
}

void system_init(void)
{
	init_mcu();

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	//USART_0_init();

	SPI_0_init();
}
