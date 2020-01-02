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
#include "Invn/Drivers/Icm426xx/Icm426xxDriver_HL.h"
/**
 * Example of using USART_0 to write "Hello World" using the IO abstraction.
 */
void USART_0_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using SPI_0 to write "Hello World" using the IO abstraction.
 */
//static uint8_t example_SPI_0[12] = "Hello World!";
int read_reg_SPI5(struct inv_icm426xx_serif * serif, uint8_t reg, uint8_t * buf, uint32_t len);
void SPI_0_example(void)
{
	
	struct inv_icm426xx_serif sensor;
	sensor.serif_type = ICM426XX_UI_SPI4;
	uint8_t whoami;
	uint8_t reg = 0x75;
	uint32_t retval = read_reg_SPI5(&sensor,reg,&whoami,1);
// 	struct io_descriptor *io;
// 	spi_m_sync_get_io_descriptor(&SPI_0, &io);
// 
// 	spi_m_sync_enable(&SPI_0);
// 	gpio_set_pin_level(SN_CS,false);
// 	uint8_t who_am_i;
// 	uint8_t reg = 0x75|0x80; /* 0x75 is whoami register, 0x80 is read flag */
// 	io_write(io, &reg, 1);
// 	int32_t returnval = io_read(io,&who_am_i,1);
// 	ASSERT(returnval==1);
// 	gpio_set_pin_level(SN_CS,true);
// 	spi_m_sync_disable(&SPI_0);
}
