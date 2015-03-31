#ifndef _MCU_LIB_H
#define _MCU_LIB_H

#include "stm32f10x.h"
#include "iic.h"
#include "mpu6050.h"

#ifdef  SERIAL_DEBUG
#include "usart.h"
#include <stdio.h>
#endif // SERIAL_DEBUG

/*****************************************************************************/
// For ADC
/*****************************************************************************/
#define L_RECEIVER    ADC_Channel_10
#define LF_RECEIVER   ADC_Channel_11
#define RF_RECEIVER   ADC_Channel_12
#define R_RECEIVER    ADC_Channel_13

#define EMITTER GPIOB

#define L_EMITTER     GPIO_Pin_15
#define LF_EMITTER    GPIO_Pin_14
#define RF_EMITTER    GPIO_Pin_13
#define R_EMITTER     GPIO_Pin_12

/*****************************************************************************/
// For Motor Control
/*****************************************************************************/
#define MOTOR       GPIOA
#define STBY        GPIO_Pin_4
#define LEFTIN1     GPIO_Pin_5
#define LEFTIN2     GPIO_Pin_6
#define RIGHTIN1    GPIO_Pin_3
#define RIGHTIN2    GPIO_Pin_2

/*****************************************************************************/
// For PWM
/*****************************************************************************/
#define PWIDTH_0      0
#define PWIDTH_25     187.5
#define PWIDTH_50     375
#define PWIDTH_75     562.5
#define PWIDTH_MAX    750

/*****************************************************************************/
// For Indications
/*****************************************************************************/
#define BUTTON   GPIO_Pin_10
#define GREEN    GPIO_Pin_4 // GPIOC
#define YELLOW   GPIO_Pin_5
#define RED      GPIO_Pin_0 //GPIOB

/*****************************************************************************/
// For Delays
/*****************************************************************************/
#define FAC_US   SystemCoreClock/8
#define FAC_MS   FAC_US*1000

/*****************************************************************************/
// Functions Prototypes
/*****************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void DMA_Configuration(void);
void ADC_Configuration(void);
void PWM_Configuration(void);

uint16_t ADC_Read(uint8_t, int);

void Delay_Init(void);
void Delay_us(uint32_t);
void Delay_ms(uint16_t);


#endif // _MCU_LIB_H
