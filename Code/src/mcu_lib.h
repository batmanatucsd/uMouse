#ifndef _MCU_LIB_H
#define _MCU_LIB_H

#include <stm32f10x.h>

/*****************************************************************************/
// For USART
/*****************************************************************************/
#define USART_BAUDRATE 115200

/*****************************************************************************/
// For PWM
/*****************************************************************************/
#define PWIDTH_0 0
#define PWIDTH_25 83
#define PWIDTH_50 166
#define PWIDTH_75 249
#define PWIDTH_80 333
#define PWIDTH_MAX 600

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

/*****************************************************************************/
// For I2C
/*****************************************************************************/

#define MPU_ADDR		0x53<<1

#define DEVID 	0x00

uint8_t buffer[6];

/*****************************************************************************/
// Functions
/*****************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);

void ADC_Configuration(void);
void PWM_Configuration(void);
void MPU_Configuration(void);

uint8_t I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr);

#ifdef  SERIAL_DEBUG
void USART_Configuration(void);
void USART_Write(uint16_t Data);
#endif // SERIAL_DEBUG

#endif // _MCU_LIB_H
