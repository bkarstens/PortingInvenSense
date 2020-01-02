/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

void delay_example(void)
{
	delay_ms(5000);
}

/**
 * Example of using SPI_5 to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_5[12] = "Hello World!";

void SPI_5_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_5, &io);

	spi_m_sync_enable(&SPI_5);
	io_write(io, example_SPI_5, 12);
}
