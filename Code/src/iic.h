#ifndef __IIC_H
#define __IIC_H

#include <stm32f10x.h>

typedef enum
{
  IIC_DMA_TX = 0,
  IIC_DMA_RX = 1
}IIC_DMADirection_TypeDef;
/*------------------------------------------------------------------------------
    Hardware Configuration
------------------------------------------------------------------------------*/
/**
  * @brief  I2C port definitions
  */
#define IIC                          I2C1
#define IIC_CLK                      RCC_APB1Periph_I2C1
#define IIC_REMAP_EN
#define IIC_REMAP                    GPIO_Remap_I2C1
#define IIC_SCL_PIN                  GPIO_Pin_8
#define IIC_SCL_GPIO_PORT            GPIOB
#define IIC_SCL_GPIO_CLK             RCC_APB2Periph_GPIOB
#define IIC_SDA_PIN                  GPIO_Pin_9
#define IIC_SDA_GPIO_PORT            GPIOB
#define IIC_SDA_GPIO_CLK             RCC_APB2Periph_GPIOB
#define IIC_DR                       ((uint32_t)0x40005810)
#define IIC_SPEED                    300000

/**
  * @brief  IIC DMA definitions
  * @REMIND Change DMA channel according to I2C
  */
#define IIC_DMA                          DMA1
#define IIC_DMA_CLK                      RCC_AHBPeriph_DMA1
#define IIC_DMA_TX_CHANNEL               DMA1_Channel6
#define IIC_DMA_RX_CHANNEL               DMA1_Channel7
#define IIC_DMA_TX_TCFLAG                DMA1_FLAG_TC6
#define IIC_DMA_RX_TCFLAG                DMA1_FLAG_TC7

/**
  * @brief  IO Expander Interrupt line on EXTI
  */
#define IIC_IT_PIN                       GPIO_Pin_12
#define IIC_IT_GPIO_PORT                 GPIOA
#define IIC_IT_GPIO_CLK                  RCC_APB2Periph_GPIOA
#define IIC_IT_EXTI_PORT_SOURCE          GPIO_PortSourceGPIOA
#define IIC_IT_EXTI_PIN_SOURCE           GPIO_PinSource12
#define IIC_IT_EXTI_LINE                 EXTI_Line12
#define IIC_IT_EXTI_IRQn                 EXTI15_10_IRQn

#define TIMEOUT_MAX 100

void IIC_GPIO_Config(void);
void IIC_Config(void);
void IIC_DMA_Config(IIC_DMADirection_TypeDef Direction, uint8_t* buffer);
void IIC_EXTI_Config(void);

uint8_t I2C_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue);
uint8_t I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr);
uint16_t I2C_ReadDataBuffer(uint8_t DeviceAddr, uint32_t RegisterAddr);


#endif // _IIC_H
