#ifndef _MCU_LIB_H
#define _MCU_LIB_H

#include "stm32f10x.h"
#include "iic.h"
#include "mpu6050.h"

/*****************************************************************************/
// For USART
/*****************************************************************************/
#define USART_BAUDRATE 9600

/*****************************************************************************/
// For ADC
/*****************************************************************************/
#define PT_LEFT     10
#define PT_LEFT45   11
#define PT_RIGHT45  12
#define PT_RIGHT    13

#define EMITTER GPIOB

#define E_LEFT     GPIO_Pin_15
#define E_LEFT45   GPIO_Pin_14
#define E_RIGHT45  GPIO_Pin_13
#define E_RIGHT    GPIO_Pin_12

/*****************************************************************************/
// For Motor Control
/*****************************************************************************/
#define MOTOR     GPIOA
#define STBY      GPIO_Pin_4
#define LEFTIN1   GPIO_Pin_5
#define LEFTIN2   GPIO_Pin_6
#define RIGHTIN1  GPIO_Pin_3
#define RIGHTIN2  GPIO_Pin_2

/*****************************************************************************/
// For PWM
/*****************************************************************************/
#define PWIDTH_0    0
#define PWIDTH_25   187.5
#define PWIDTH_50   375
#define PWIDTH_75   562.5
#define PWIDTH_MAX  750

/*****************************************************************************/
// For Indications
/*****************************************************************************/
#define BUTTON  GPIO_Pin_10
#define GREEN   GPIO_Pin_4 // GPIOC
#define YELLOW  GPIO_Pin_5
#define RED     GPIO_Pin_0 //GPIOB

/*****************************************************************************/
// Functions
/*****************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void ADC_Configuration(void);
void PWM_Configuration(void);

uint16_t ADC_Read(uint8_t channel);

#ifdef  SERIAL_DEBUG
void USART_Configuration(void);
void USART_Write(uint16_t Data);
uint8_t USART_Read(void);
void USART_SendInt(uint16_t num);
#endif // SERIAL_DEBUG

#endif // _MCU_LIB_H
