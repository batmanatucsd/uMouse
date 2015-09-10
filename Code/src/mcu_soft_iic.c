#include "mcu_soft_iic.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdint.h>
#include "mcu_delay.h"

#define I2C_READY		0x00
#define I2C_BUS_BUSY	0x01
#define I2C_BUS_ERROR	0x02

#define I2C_NACK    0x00
#define I2C_ACK     0x01

#define I2C_Direction_Transmitter   0x00
#define I2C_Direction_Receiver      0x01

static int I2C_START(void)
{
	gpio_set(I2C_port, I2C_SDA);
 	delay_us(10);

 	gpio_set(I2C_port, I2C_SCL);
 	delay_us(10);

 	if(!gpio_get(I2C_port, I2C_SDA)) return I2C_BUS_BUSY;

 	gpio_clear(I2C_port, I2C_SDA);
 	delay_us(10);

 	gpio_clear(I2C_port, I2C_SCL);
 	delay_us(10);

 	if(gpio_get(I2C_port, I2C_SDA)) return I2C_BUS_ERROR;

 	return I2C_READY;
}

static void I2C_STOP(void)
{
 	gpio_clear(I2C_port, I2C_SDA);
 	delay_us(10);

 	gpio_set(I2C_port, I2C_SCL);
 	delay_us(10);

 	gpio_set(I2C_port, I2C_SDA);
 	delay_us(10);
}

static void I2C_SendACK(void)
{
 	gpio_clear(I2C_port, I2C_SDA);
 	delay_us(10);

 	gpio_set(I2C_port, I2C_SCL);
 	delay_us(10);

 	gpio_clear(I2C_port, I2C_SCL);
 	delay_us(10);
}

static void I2C_SendNACK(void)
{
	gpio_set(I2C_port, I2C_SDA);
	delay_us(10);

	gpio_set(I2C_port, I2C_SCL);
	delay_us(10);

	gpio_clear(I2C_port, I2C_SCL);
	delay_us(10);
}

static uint8_t I2C_SendByte(uint8_t i2c_data)
{
 	uint8_t i;

	gpio_clear(I2C_port, I2C_SCL);

 	for(i=0; i<8; i++)
 	{
  		if(i2c_data&0x80)
            gpio_set(I2C_port, I2C_SDA);
   		else
            gpio_clear(I2C_port, I2C_SDA);

  		i2c_data<<=1;
  		delay_us(10);

  		gpio_set(I2C_port, I2C_SCL);
  		delay_us(10);
  		gpio_clear(I2C_port, I2C_SCL);
  		delay_us(10);
 	}

 	gpio_set(I2C_port, I2C_SDA);
 	delay_us(10);

 	gpio_set(I2C_port, I2C_SCL);
 	delay_us(10);

 	if(gpio_get(I2C_port, I2C_SDA))
 	{
  		gpio_clear(I2C_port, I2C_SCL);
  		return I2C_NACK;
 	}
 	else
 	{
  		gpio_clear(I2C_port, I2C_SCL);
  		return I2C_ACK;
 	}
}

static uint8_t I2C_ReceiveByte(void)
{
	uint8_t i, i2c_data = 0;

 	gpio_set(I2C_port, I2C_SDA);
 	gpio_clear(I2C_port, I2C_SCL);

 	for(i=0; i<8; i++)
 	{
  		gpio_set(I2C_port, I2C_SCL);
  		delay_us(10);

  		i2c_data<<=1;

  		if(gpio_get(I2C_port, I2C_SDA))
            i2c_data|=0x01;

  		gpio_clear(I2C_port, I2C_SCL);
  		delay_us(10);
 	}

	I2C_SendNACK();

    return i2c_data;
}

static uint8_t I2C_ReceiveByte_WithACK(void)
{
	uint8_t i, i2c_data = 0;

 	gpio_set(I2C_port, I2C_SDA);
 	gpio_clear(I2C_port, I2C_SCL);

 	for(i=0; i<8; i++)
 	{
  		gpio_set(I2C_port, I2C_SCL);
  		delay_us(10);

  		i2c_data<<=1;

  		if(gpio_get(I2C_port, I2C_SDA))
            i2c_data|=0x01;

  		gpio_clear(I2C_port, I2C_SCL);
  		delay_us(10);
 	}

	I2C_SendACK();

 	return i2c_data;
}

void I2C_init(void)
{
    rcc_periph_clock_enable(I2C_RCC_port);

    gpio_set_mode(I2C_port, GPIO_MODE_OUTPUT_2_MHZ,
        GPIO_CNF_OUTPUT_OPENDRAIN, I2C_SCL | I2C_SDA);

	gpio_set(I2C_port, I2C_SCL);
	gpio_set(I2C_port, I2C_SDA);
	delay_ms(50);
}


int I2C_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data)
{
	uint8_t i;
	I2C_START();

	I2C_SendByte(slave_addr << 1 | I2C_Direction_Transmitter);

	I2C_SendByte(reg_addr);

	for (i=0;i<length;i++) I2C_SendByte(data[i]);

	I2C_STOP();
	return 0x00;
}

int I2C_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data)
{
    I2C_START();

	I2C_SendByte(slave_addr << 1 | I2C_Direction_Transmitter);

	I2C_SendByte(reg_addr);

	I2C_START();

	I2C_SendByte(slave_addr << 1 | I2C_Direction_Receiver);

    while (length)
	{
		if (length==1)
            *data =I2C_ReceiveByte();
        else
            *data =I2C_ReceiveByte_WithACK();
        data++;
        length--;
    }

	I2C_STOP();
    return 0x00;
}
