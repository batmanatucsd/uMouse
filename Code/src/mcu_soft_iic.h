#ifndef _MCU_SOFT_IIC_H_
#define _MCU_SOFT_IIC_H_

#define I2C_RCC_port    RCC_GPIOB
#define I2C_port    GPIOB
#define I2C_SDA     GPIO_I2C1_RE_SDA
#define I2C_SCL     GPIO_I2C1_RE_SCL

void I2C_init(void);

/**
 *  @brief      Write to a device register.
 *
 *  @param[in]  slave_addr  Slave address of device.
 *  @param[in]  reg_addr	Slave register to be written to.
 *  @param[in]  length      Number of bytes to write.
 *  @param[out] data        Data to be written to register.
 *
 *  @return     0 if successful.
 */
int I2C_write(unsigned char slave_addr,
    unsigned char reg_addr,
    unsigned char length,
    unsigned char const *data);

/**
 *  @brief      Read from a device.
 *
 *  @param[in]  slave_addr  Slave address of device.
 *  @param[in]  reg_addr	Slave register to be read from.
 *  @param[in]  length      Number of bytes to read.
 *  @param[out] data        Data from register.
 *
 *  @return     0 if successful.
 */
int I2C_read(unsigned char slave_addr,
    unsigned char reg_addr,
    unsigned char length,
    unsigned char *data);

#endif
