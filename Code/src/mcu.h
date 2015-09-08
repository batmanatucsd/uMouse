#ifndef _MCU_H_
#define _MCU_H_

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>


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
#define L_PWM         TIM3
#define R_PWM         TIM5
#define PWIDTH_0      0
#define PWIDTH_25     375
#define PWIDTH_50     750
#define PWIDTH_75     1125
#define PWIDTH_MAX    1500

/*****************************************************************************/
// For Indications
/*****************************************************************************/
#define BUTTON   GPIO_Pin_10
#define GREEN    GPIO_Pin_4 // GPIOC
#define YELLOW   GPIO_Pin_5
#define RED      GPIO_Pin_0 //GPIOB

/*****************************************************************************/
// For Encoders
/*****************************************************************************/
#define L_ENC   TIM8
#define R_ENC   TIM4


/*****************************************************************************/
// Functions Prototypes
/*****************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void DMA_Configuration(void);
void ADC_Configuration(void);
void PWM_Configuration(void);
void NVIC_Configuration(void);
void ENCODER_Configuration(void);

void ADC_Read(uint16_t, uint16_t, uint16_t, uint16_t);

void Delay_Init(void);
void Decrement_WaitTime(void);
void Delay_us(uint32_t);
void Delay_ms(uint16_t);


#endif // _MCU_LIB_H
