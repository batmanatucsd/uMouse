#include "mcu_iic.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/cm3/cortex.h>

void i2c_init()
{
 /* Enable clocks for I2C1 and AFIO. */
 rcc_periph_clock_enable(RCC_I2C1);
 rcc_periph_clock_enable(RCC_AFIO);

 /* Set alternate functions for the SCL and SDA pins of I2C1. */
 gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
         GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
         GPIO_I2C1_SCL | GPIO_I2C1_SDA);

 /* Disable the I2C before changing any configuration. */
 i2c_peripheral_disable(I2C1);

 /* APB1 is running at 36MHz. */
 i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);

 /* 400KHz - I2C Fast Mode */
 i2c_set_fast_mode(I2C1);

 /*
  * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
  * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
  * Datasheet suggests 0x1e.
  */
 i2c_set_ccr(I2C1, 0x1e);

 /*
  * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
  * 400kHz => 300ns and 100kHz => 1000ns; 300ns/28ns = 10;
  * Incremented by 1 -> 11.
  */
 i2c_set_trise(I2C1, 0x0b);

 /*
  * This is our slave address - needed only if we want to receive from
  * other masters.
  */
 i2c_set_own_7bit_slave_address(I2C1, 0x32);

 /* If everything is configured -> enable the peripheral. */
 i2c_peripheral_enable(I2C1);
}

int i2c_write(unsigned char slave_addr, unsigned char reg_addr,
    unsigned char length, unsigned char const *data)
{
    uint32_t reg32 __attribute__((unused));

    /* Send START condition. */
    i2c_send_start(I2C1);

    /* Waiting for START is send and switched to master mode. */
    while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
        & (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

    /* Send destination address. */
    i2c_send_7bit_address(I2C1, slave_addr, I2C_WRITE);

    /* Waiting for address is transferred. */
    while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));

    /* Cleaning ADDR condition sequence. */
    reg32 = I2C_SR2(I2C1);

    /* Sending the data. */
    i2c_send_data(I2C1, reg_addr);
    while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));

    while (length-- > 1) {
        i2c_send_data(I2C1, (uint8_t)(*(data++)));
        while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));
    }

    i2c_send_data(I2C1, (uint8_t)(*data));
    /* After the last byte we have to wait for TxE too. */
    while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));

    /* Send STOP condition. */
    i2c_send_stop(I2C1);

    return 0;
}

int i2c_read(unsigned char slave_addr, unsigned char reg_addr,
    unsigned char length, unsigned char *data)
{
    uint32_t reg32 __attribute__((unused));

	/* Send START condition. */
	i2c_send_start(I2C1);

	/* Waiting for START is send and switched to master mode. */
	while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
		& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

	/* Say to what address we want to talk to. */
	/* Yes, WRITE is correct - for selecting register in STTS75. */
	i2c_send_7bit_address(I2C1, slave_addr, I2C_WRITE);

	/* Waiting for address is transferred. */
	while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));

	/* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(I2C1);

	i2c_send_data(I2C1, reg_addr);
	while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));

    /* Send START condition. */
    i2c_send_start(I2C1);

    /* Waiting for START is send and switched to master mode. */
    while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
        & (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

    /* Say to what address we want to talk to. */
    i2c_send_7bit_address(I2C1, slave_addr, I2C_READ);

    if(length > 2)
    {
        /* Waiting for address is transferred. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));

        /* Cleaning ADDR condition sequence. */
        reg32 = I2C_SR2(I2C1);

        while(length-- > 3)
        {
            /* Waiting for EV7 software sequence. */
            while (!(I2C_SR1(I2C1) & I2C_SR1_RxNE));

            /* Cleaning EV7 software sequence. */
            *(data++) = I2C_DR(I2C1);
        }

        /* Waiting for EV7_2 software sequence. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));

        /* Cleaning I2C_SR1_ACK. */
        I2C_CR1(I2C1) &= ~I2C_CR1_ACK;

        /* Reading DataN-2 */
        *(data++) = I2C_DR(I2C1);

        /* Disable interrupts. */
        cm_disable_interrupts();

        /* STOP = 1 */
        I2C_CR1(I2C1) |= I2C_CR1_STOP;

        /* Reading DataN-1 */
        *(data++) = I2C_DR(I2C1);

        /* Enable interrupts. */
        cm_enable_interrupts();

        /* Waiting for EV7 software sequence. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_RxNE));

        /* Reading DataN */
        *data = I2C_DR(I2C1);

        /*Wait until STOP is cleared by hardware. */
        while ((I2C_CR1(I2C1) & I2C_CR1_STOP) == I2C_CR1_STOP)

        /* ACK = 1 */
        I2C_CR1(I2C1) |= I2C_CR1_ACK;

        return 0;
    }
    else if(length == 2)
    {
        /* Waiting for address is transferred. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));

        /* Set POS */
        I2C_CR1(I2C1) |= I2C_CR1_POS;

        /* Disable interrupts. */
        cm_disable_interrupts();

        /* Cleaning ADDR condition sequence. */
        reg32 = I2C_SR2(I2C1);

        /* Cleaning I2C_SR1_ACK. */
        I2C_CR1(I2C1) &= ~I2C_CR1_ACK;

        /* Enable interrupts. */
        cm_enable_interrupts();

        /* Now the slave should begin to send us the first byte. Await BTF. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_BTF));

        /* Disable interrupts. */
        cm_disable_interrupts();

        /* STOP = 1 */
        I2C_CR1(I2C1) |= I2C_CR1_STOP;

        /* Reading Data1 */
        *data = I2C_DR(I2C1);

        /* Enable interrupts. */
        cm_enable_interrupts();

        /* Reading Data2 */
        *(++data) = I2C_DR(I2C1);

        /*Wait until STOP is cleared by hardware. */
        while ((I2C_CR1(I2C1) & I2C_CR1_STOP) == I2C_CR1_STOP)

        /* POS = 0 and ACK = 1 */
        I2C_CR1(I2C1) &= ~I2C_CR1_POS;
        I2C_CR1(I2C1) |= I2C_CR1_ACK;

        return 0;
    }
    else
    {
        /* Waiting for address is transferred. */
        while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));

        /* Cleaning I2C_SR1_ACK. */
        I2C_CR1(I2C1) &= ~I2C_CR1_ACK;

        /* Disable interrupts. */
        cm_disable_interrupts();

        /* Cleaning ADDR condition sequence. */
        reg32 = I2C_SR2(I2C1);

        /* STOP = 1 */
        I2C_CR1(I2C1) |= I2C_CR1_STOP;

        /* Enable interrupts. */
        cm_enable_interrupts();

        /* Wait until RxNE = 1 */
        while (!(I2C_SR1(I2C1) & I2C_SR1_RxNE));

        /* Reading Data */
        *data = I2C_DR(I2C1);

        /*Wait until STOP is cleared by hardware. */
        while ((I2C_CR1(I2C1) & I2C_CR1_STOP) == I2C_CR1_STOP)

        /* ACK = 1 */
        I2C_CR1(I2C1) |= I2C_CR1_ACK;

        return 0;
    }
}
