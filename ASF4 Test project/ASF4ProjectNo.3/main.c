#include <atmel_start.h>
#include "examples/driver_examples.h"
#include "Invn/Drivers/Icm426xx/Icm426xxDriver_HL.h"


int read_reg_SPI5(struct inv_icm426xx_serif * serif, uint8_t reg, uint8_t * buf, uint32_t len)
{
	uint32_t status = 0;
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_0, &io);
	ASSERT(serif->serif_type == ICM426XX_UI_SPI4);

	spi_m_sync_enable(&SPI_0);                   /* enable SPI communication                           */
	gpio_set_pin_level(SN_CS,false);             /* enable chip select (active low)                    */
	reg |= 0x80;                                 /* set the read flag                                  */
	status |= io_write(io, &reg, 1);             /* tell the chip that we want to read from a register */
	status |= io_read(io, buf, len);             /* read from the register                             */
	gpio_set_pin_level(SN_CS,true);              /* disable chip select (active low)                   */
	spi_m_sync_disable(&SPI_0);                  /* disable SPI communication                          */
	return status;
}
int write_reg_SPI5(struct inv_icm426xx_serif * serif, uint8_t reg, const uint8_t * buf, uint32_t len)
{
	uint32_t status = 0;
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_0, &io);
	ASSERT(serif->serif_type == ICM426XX_UI_SPI4);

	spi_m_sync_enable(&SPI_0);                   /* enable SPI communication                           */
	gpio_set_pin_level(SN_CS,false);             /* enable chip select (active low)                    */
	status |= io_write(io, &reg, 1);             /* tell the chip that we want to write to a register  */
	status |= io_write(io, buf, len);            /* write to the register                              */
	gpio_set_pin_level(SN_CS,true);              /* disable chip select (active low)                   */
	spi_m_sync_disable(&SPI_0);                  /* disable SPI communication                          */
	return status;
}

static void setupInvensense(struct inv_icm426xx * p_sensor)
{
	struct inv_icm426xx_serif icm_serif;
	icm_serif.context   = 0;        /* no need */
	icm_serif.read_reg  = read_reg_SPI5;
	icm_serif.write_reg = write_reg_SPI5;
	icm_serif.max_read  = 1024*32;  /* maximum number of bytes allowed per serial read  (from InvenSense examples) */
	icm_serif.max_write = 1024*32;  /* maximum number of bytes allowed per serial write (from InvenSense examples) */
	icm_serif.serif_type = ICM426XX_UI_SPI4;
	int retval = inv_icm426xx_init(p_sensor,&icm_serif,NULL);
	ASSERT(retval==0);
}

int main(void)
{
	struct inv_icm426xx sensor;
	uint8_t whoami;
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	//setupInvensense(&sensor);
	SPI_0_example();
	/* Replace with your application code */
	while (1) {
		
		//int retval = inv_icm426xx_get_who_am_i(&sensor,&whoami);
		//ASSERT(retval==0);
	}
}
