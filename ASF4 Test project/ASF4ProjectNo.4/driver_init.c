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

struct spi_m_sync_descriptor SPI_5;

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void SPI_5_PORT_init(void)
{

	gpio_set_pin_function(PA12, MUX_PA12A_FLEXCOM5_MISO);

	gpio_set_pin_function(PA13, MUX_PA13A_FLEXCOM5_MOSI);

	gpio_set_pin_function(PA14, MUX_PA14A_FLEXCOM5_SPCK);
}

void SPI_5_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_FLEXCOM5);
}

void SPI_5_init(void)
{
	SPI_5_CLOCK_init();
	spi_m_sync_set_func_ptr(&SPI_5, NULL);
	spi_m_sync_init(&SPI_5, FLEXCOM5);
	SPI_5_PORT_init();
}

void system_init(void)
{
	init_mcu();

	_pmc_enable_periph_clock(ID_PIOA);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PA5 */

	gpio_set_pin_level(SN_CS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(SN_CS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SN_CS, GPIO_PIN_FUNCTION_OFF);

	delay_driver_init();

	SPI_5_init();
}
