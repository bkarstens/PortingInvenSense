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
#include <hpl_tc.h>

struct timer_descriptor      TIMER_0;
struct spi_m_sync_descriptor SPI_5;

void EXTERNAL_IRQ_0_init(void)
{

	// Set pin direction to input
	gpio_set_pin_direction(INT1, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(INT1,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(INT1, GPIO_PIN_FUNCTION_OFF);
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

/**
 * \brief Timer initialization function
 *
 * Enables Timer peripheral, clocks and initializes Timer driver
 */
static void TIMER_0_init(void)
{
	_pmc_enable_periph_clock(ID_TC0_CHANNEL0);
	timer_init(&TIMER_0, TC0, _tc_get_timer());
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

	_pmc_enable_periph_clock(ID_PIOB);

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

	EXTERNAL_IRQ_0_init();

	delay_driver_init();
	TIMER_0_init();

	SPI_5_init();

	ext_irq_init();
}
