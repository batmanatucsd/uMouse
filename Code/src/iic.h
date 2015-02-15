#ifndef _IIC_H
#define _IIC_H

#include <stm32f10x.h>

//I2C Universal Requirement
#define IIC_RX_DMA_Channel DMA1_Channel7 //note the NVIC is not change simultaneously
#define IIC_DR_Address 0x40005410
#define IIC_RX_BUFF_SIZE 1

//I2C Slave Address
#define MPU_ADDR  0x53<<1

//I2C Slave Register
#define DEVID 0x00

#define IIC_DMA_EN 6

extern uint8_t IIC_RX_Buffer[];
extern uint8_t IIC_RX_OUTPUT;

void sensor_init(void);

void IIC_Configuration(void);
void IIC_DMA_Configuration(void);
void IIC_NVIC_Configuration(void);
uint8_t IIC_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr);
void IIC_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t Data);

void IIC_DMA_Read(uint8_t DeviceAddr, uint8_t readAddr);
#endif // _IIC_H
