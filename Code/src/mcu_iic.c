#include "mcu_iic.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/cm3/cortex.h>

#include "mcu_delay.h"
#include <stdio.h>

void I2C_init()
{
    /* Enable clocks for I2C1 and AFIO. */
    rcc_periph_clock_enable(I2C_RCC_port | RCC_AFIO | RCC_I2C1);

    /* Set alternate functions(REMAP) for the SCL and SDA pins of I2C1. */
    gpio_set_mode(I2C_port, GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, I2C_SCL | I2C_SDA);

    /* Set I2C remap and disable full jtag funcion */
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, AFIO_MAPR_I2C1_REMAP);

    /* Disable the I2C before changing any configuration. */
    i2c_peripheral_disable(I2C1);

    /* PCLK must be a multiple of 10Mhz to reach 400KHz */
    i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_30MHZ);

    /* 400KHz - I2C Fast Mode */
    i2c_set_fast_mode(I2C1);

    i2c_set_dutycycle(I2C1, I2C_CCR_DUTY_DIV2);

    /*
     * pclk for I2C is 30MHz -> cycle time 33.3ns
     * Thigh + Tlow = 3 * CCR * Tpclk
     * ->one cycle time at 400kHz = 1 / 400 kHz = 2500 ns
     * CCR = 25d = 0x19
     */
    i2c_set_ccr(I2C1, 0x19);

    /*
     * pclk for I2C is 30MHz -> cycle time 33.3ns
     * rise time for 400kHz => 300ns and 100kHz => 1000ns;
     * 300ns/33.3ns = 9
     * Incremented by 1 -> 10
     */
    i2c_set_trise(I2C1, 0x0A);

    i2c_enable_interrupt(I2C1, I2C_CR2_ITERREN);

    /* If everything is configured -> enable the peripheral. */
    i2c_peripheral_enable(I2C1);

    delay_ms(50);

    if ((I2C_SR2(I2C1) & I2C_SR2_BUSY)) {

       while(1);
   }
}

int I2C_write(unsigned char slave_addr, unsigned char reg_addr,
    unsigned char length, unsigned char const *data)
{
    uint32_t reg32 __attribute__((unused));

    if ((I2C_SR2(I2C1) & I2C_SR2_BUSY)) {

       while(1);
   }

    /* Send START condition. */
    i2c_send_start(I2C1);

    /* Waiting for START is send and switched to master mode. */
    while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
        & (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))))
    {
        printf("SR1:%X SR2:%X \n", (unsigned int)I2C_SR1(I2C1), (unsigned int)I2C_SR2(I2C1));
    }

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

int I2C_read(unsigned char slave_addr, unsigned char reg_addr,
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
