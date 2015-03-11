#ifndef _IIC_H
#define _IIC_H

#include <stm32f10x.h>

//I2C Slave Address
#define MPU_ADDR  0x53<<1

//I2C Slave Register
#define DEVID 0x00

//#define IIC_DMA_EN 1

//I2C Universal Requirement
#define IIC_CLK RCC_APB1Periph_I2C1
#define IIC_GPIO_CLK RCC_APB2Periph_GPIOB
#define IIC_SCL GPIO_Pin_8
#define IIC_SDA GPIO_Pin_9

//IIC DMA Direction
typedef enum
{
  IIC_DMA_TX = 0,
  IIC_DMA_RX = 1
}IIC_DMADirection_TypeDef;

//I2C DMA Requirement
#define IIC_DMA_CLK RCC_AHBPeriph_DMA1
#define IIC_DR_Addr ((uint32_t)0x40005810)
#define IIC_DMA_TX_CHANNEL DMA1_Channel6 //The channel need to be change for different
#define IIC_DMA_RX_CHANNEL DMA1_Channel7


extern uint8_t IIC_RX_Buffer[];
extern uint8_t IIC_RX_OUTPUT;

void sensor_init(void);

void IIC_Configuration(void);
void IIC_NVIC_Configuration(void);

uint8_t IIC_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr);
void IIC_DMA_Read(uint8_t DeviceAddr, uint8_t readAddr);

void IIC_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t Data);
#endif // _IIC_H
