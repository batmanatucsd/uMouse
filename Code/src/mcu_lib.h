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

#define MPU_ADDR 0x53<<1

uint8_t buffer[6];

/*****************************************************************************/
// Functions
/*****************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void ADC_Configuration(void);
void PWM_Configuration(void);
void MPU_Configuration(void);

#endif // _MCU_LIB_H
