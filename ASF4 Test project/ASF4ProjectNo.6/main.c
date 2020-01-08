#include <atmel_start.h>
#include <stdlib.h>
#include "Invn/Drivers/Icm426xx/Icm426xxDriver_HL.h"
#include "hri_tc_g55.h"

typedef int (*read_reg_t)(struct inv_icm426xx_serif *, uint8_t, uint8_t *, uint32_t);
typedef int (*write_reg_t)(struct inv_icm426xx_serif *, uint8_t, uint8_t const *, uint32_t);

/*
 * Icm426xx driver needs a sleep feature from external device. Thus inv_icm426xx_sleep_us
 * is defined as extern symbol in driver. Let's give its implementation here.
 */
void inv_icm426xx_sleep_us(uint32_t us)
{
	delay_us(us);
}

/*
 * Icm426xx driver needs to get time in us. Let's give its implementation here.
 */
uint64_t inv_icm426xx_get_time_us(void)
{
	// in Config/hpl_tc_config.h timer is configured to tick every 1000us. Ticking every us bogs everything down.
	return TIMER_0.time * (uint64_t)1000;
}

// Unnecessary b/c burst read works
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// int read_reg_1(struct inv_icm426xx_serif * serif, uint8_t reg, uint8_t * buf, uint32_t len)
// {
// 	int address_written = 1;
// 	int bytes_read = 0;
// 	struct io_descriptor *io;
// 	spi_m_sync_get_io_descriptor(serif->context, &io);
// 	ASSERT(serif->serif_type == ICM426XX_UI_SPI4);
// 	spi_m_sync_enable(serif->context);                 /* enable SPI communication                           */
// 	for (uint8_t i = 0; i < len; ++i)                  /* Read from each register                            */
// 	{
// 		gpio_set_pin_level(SN_CS, false);              /* enable chip select (active low)                    */
// 		uint8_t addr = (reg + i) | 0x80;               /* set the read flag                                  */
// 		address_written &= io_write(io, &addr, 1);     /* tell the chip that we want to read from a register */
// 		bytes_read += io_read(io, &buf[i], 1);         /* read from the register                             */
// 		gpio_set_pin_level(SN_CS, true);               /* disable chip select (active low)                   */
// 	}
// 	spi_m_sync_disable(serif->context);                /* disable SPI communication                          */
// 	return !(address_written==1 && bytes_read==len);
// }

// Burst write does not work. Example project just writes one byte at a time in a loop; I guess it's because it cant burst write?
// see inv_io_hal_write_reg in invensense\sources\examples\system-interface\system-interface.c
int write_reg_1(struct inv_icm426xx_serif * serif, uint8_t reg, const uint8_t * buf, uint32_t len)
{
	int address_written = 1;
	int bytes_written = 0;
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(serif->context, &io);
	ASSERT(serif->serif_type == ICM426XX_UI_SPI4);

	spi_m_sync_enable(serif->context);               /* enable SPI communication                           */
	for (uint8_t i = 0; i < len; ++i)                /* Write each byte separately                         */
	{
		gpio_set_pin_level(SN_CS,false);             /* enable chip select (active low)                    */
		uint8_t addr = (reg + i) & 0x7F;			 /* make sure the read flag is not set                 */
		address_written &= io_write(io, &addr, 1);   /* tell the chip that we want to write to a register  */
		bytes_written   += io_write(io, &buf[i], 1); /* write to the register                              */
		gpio_set_pin_level(SN_CS, true);             /* disable chip select (active low)                   */
	}
	spi_m_sync_disable(serif->context);              /* disable SPI communication                          */
	return !(address_written==1 && bytes_written==len);
}

int read_reg_2(struct inv_icm426xx_serif * serif, uint8_t reg, uint8_t * buf, uint32_t len)
{
	int address_written;
	int bytes_read;
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(serif->context, &io);
	ASSERT(serif->serif_type == ICM426XX_UI_SPI4);

	spi_m_sync_enable(serif->context);             /* enable SPI communication                           */
	gpio_set_pin_level(SN_CS,false);               /* enable chip select (active low)                    */
	uint8_t addr = reg | 0x80;                     /* set the read flag                                  */
	address_written = io_write(io, &addr, 1);      /* tell the chip that we want to read from a register */
	bytes_read      = io_read(io,    buf, len);    /* read from the register                             */
	gpio_set_pin_level(SN_CS,true);                /* disable chip select (active low)                   */
	spi_m_sync_disable(serif->context);            /* disable SPI communication                          */
	return !(address_written==1 && bytes_read==len);
}

// This does not work. Example project just writes one byte at a time in a loop; I guess it's because it cant burst write?
// see inv_io_hal_write_reg in invensense\sources\examples\system-interface\system-interface.c
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// int write_reg_2(struct inv_icm426xx_serif * serif, uint8_t reg, const uint8_t * buf, uint32_t len)
// {
// 	int address_written;
// 	int bytes_written;
// 	struct io_descriptor *io;
// 	spi_m_sync_get_io_descriptor(serif->context, &io);
// 	ASSERT(serif->serif_type == ICM426XX_UI_SPI4);
// 
// 	spi_m_sync_enable(serif->context);                /* enable SPI communication                           */
// 	gpio_set_pin_level(SN_CS,false);                  /* enable chip select (active low)                    */
// 	address_written = io_write(io, &reg, 1);          /* tell the chip that we want to write to a register  */
// 	bytes_written   = io_write(io,  buf, len);        /* write to the register                              */
// 	gpio_set_pin_level(SN_CS,true);                   /* disable chip select (active low)                   */
// 	spi_m_sync_disable(serif->context);               /* disable SPI communication                          */
// 	return !(address_written==1 && bytes_written==len);
// }


uint16_t accel_data[3];
void filler_event_handler(inv_icm426xx_sensor_event_t * event)
{
	accel_data[0] = event->accel[0];
	accel_data[1] = event->accel[1];
	accel_data[2] = event->accel[2];
}

// copied from invensense/sources/examples/example-raw-data-registers/example-raw-data-registers.c
static int ConfigureInvDevice(struct inv_icm426xx *p_sensor,
	uint8_t is_low_noise_mode,
	ICM426XX_ACCEL_CONFIG0_FS_SEL_t acc_fsr_g,
	ICM426XX_GYRO_CONFIG0_FS_SEL_t gyr_fsr_dps,
	ICM426XX_ACCEL_CONFIG0_ODR_t acc_freq,
	ICM426XX_GYRO_CONFIG0_ODR_t gyr_freq,
	uint8_t is_rtc_mode)
{
	int rc = 0;
	
	rc |= inv_icm426xx_enable_clkin_rtc(p_sensor, is_rtc_mode);

	rc |= inv_icm426xx_set_accel_fsr(p_sensor, acc_fsr_g);
	rc |= inv_icm426xx_set_gyro_fsr(p_sensor, gyr_fsr_dps);
	
	rc |= inv_icm426xx_set_accel_frequency(p_sensor, acc_freq);
	rc |= inv_icm426xx_set_gyro_frequency(p_sensor, gyr_freq);
	
	if (is_low_noise_mode)
	rc |= inv_icm426xx_enable_accel_low_noise_mode(p_sensor);
	else
	rc |= inv_icm426xx_enable_accel_low_power_mode(p_sensor);
	
	rc |= inv_icm426xx_enable_gyro_low_noise_mode(p_sensor);

	/* Wait Max of ICM426XX_GYR_STARTUP_TIME_US and ICM426XX_ACC_STARTUP_TIME_US*/
	(ICM426XX_GYR_STARTUP_TIME_US > ICM426XX_ACC_STARTUP_TIME_US) ? inv_icm426xx_sleep_us(ICM426XX_GYR_STARTUP_TIME_US) : inv_icm426xx_sleep_us(ICM426XX_ACC_STARTUP_TIME_US);
	
	return rc;
}

static void setup_invensense(struct inv_icm426xx * p_sensor)
{
	struct inv_icm426xx_serif icm_serif;
	icm_serif.context   = &SPI_5;      /* using context as the spi_m_sync_descriptor to use                           */
	icm_serif.read_reg  = read_reg_2;  /* use the burst read fn                                                       */
	icm_serif.write_reg = write_reg_1; /* use the for loop write fn b/c the burst write fn doesn't work               */
	icm_serif.max_read  = 1024*32;     /* maximum number of bytes allowed per serial read  (from InvenSense examples) */
	icm_serif.max_write = 1024*32;     /* maximum number of bytes allowed per serial write (from InvenSense examples) */
	icm_serif.serif_type = ICM426XX_UI_SPI4;
	int retval = inv_icm426xx_init(p_sensor,&icm_serif,filler_event_handler);
 	retval |= inv_icm426xx_configure_fifo(p_sensor, INV_ICM426XX_FIFO_DISABLED);
	retval |= ConfigureInvDevice(p_sensor,
								 (uint8_t )1,
	                             ICM426XX_ACCEL_CONFIG0_FS_SEL_4g,
	                             ICM426XX_GYRO_CONFIG0_FS_SEL_2000dps,
	                             ICM426XX_ACCEL_CONFIG0_ODR_1_KHZ,
	                             ICM426XX_GYRO_CONFIG0_ODR_1_KHZ,
								 (uint8_t )0);
	ASSERT(retval==INV_ERROR_SUCCESS);
}

void beesert(uint32_t condition)
{
	if (!condition)
	for(;;);
}

void testReadWrite(read_reg_t read_fn, write_reg_t write_fn)
{
	struct inv_icm426xx_serif icm_serif;
	icm_serif.context   = &SPI_5;   /* using context as the spi_m_sync_descriptor to use */
	icm_serif.read_reg  = read_fn;
	icm_serif.write_reg = write_fn;
	icm_serif.max_read  = 1024*32;  /* maximum number of bytes allowed per serial read  (from InvenSense examples) */
	icm_serif.max_write = 1024*32;  /* maximum number of bytes allowed per serial write (from InvenSense examples) */
	icm_serif.serif_type = ICM426XX_UI_SPI4;
	uint8_t buf [3]= {0,0x42,0xFF};
	uint32_t status = 0;
	/************************************************************************/
	/* Singe Read                                                           */
	/************************************************************************/
	status |= read_fn(&icm_serif,MPUREG_WHO_AM_I,buf,1);
	beesert(buf[0]==0x42);
	
	/************************************************************************/
	/* Multiple Read/Write                                                  */
	/************************************************************************/
	buf[0]=4;
	status |= write_fn(&icm_serif, MPUREG_REG_BANK_SEL,buf,1);      // switch to bank 4
	status |= read_fn(&icm_serif, MPUREG_OFFSET_USER_0_B4, buf, 2); /* burst read OFFSET_USER0 and OFFSET_USER1 registers */
	status |= read_fn(&icm_serif, MPUREG_OFFSET_USER_1_B4, &buf[2], 1);
	buf[0] = 0xAB;
	buf[1] = 0xCD;
	status |= write_fn(&icm_serif, MPUREG_OFFSET_USER_0_B4, buf, 2); /* burst write OFFSET_USER0 and OFFSET_USER1 registers */
	buf[0]=0xFF;
	buf[1]=0xFF;
	buf[2]=0xFF;
	status |= read_fn(&icm_serif, MPUREG_OFFSET_USER_0_B4, buf, 2); /* burst read OFFSET_USER0 and OFFSET_USER1 registers */
	status |= read_fn(&icm_serif, MPUREG_OFFSET_USER_1_B4, &buf[2], 1);
	beesert(buf[0]==0xAB);
	beesert(buf[1]==0xCD);
	beesert(buf[1]==buf[2]);
	
	beesert(status==0);
}
uint64_t current_time;

static struct inv_icm426xx sensor;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	//testReadWrite(read_reg_2,write_reg_1);
	
	timer_start(&TIMER_0);
	
	/* sets up InvenSense structure and chip */
	setup_invensense(&sensor);
	
	/* verifying time is correct */
	current_time = inv_icm426xx_get_time_us();
	delay_ms(1000);
	current_time = inv_icm426xx_get_time_us() - current_time;
	
	/* Replace with your application code */
	while (1) {
		delay_ms(500);
		//uint32_t retval = inv_icm426xx_get_data_from_fifo(sensor);
		inv_icm426xx_get_data_from_registers(&sensor);
	}
}
