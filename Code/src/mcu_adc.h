#ifndef _MCU_ADC_H_
#define _MCU_ADC_H_

#include <stdint.h>

#define REC_RCC_PORT    RCC_GPIOC
#define REC_PORT        GPIOC

#define L_REC_PIN   GPIO0
#define LF_REC_PIN  GPIO1
#define RF_REC_PIN  GPIO2
#define R_REC_PIN   GPIO3

#define EMIT_RCC_PORT   RCC_GPIOB
#define EMIT_PORT       GPIOB

#define L_EMIT_PIN      GPIO_15
#define LF_EMIT_PIN     GPIO_14
#define RF_EMIT_PIN     GPIO_13
#define R_EMIT_PIN      GPIO_12

#define REC_RCC_DMA RCC_DMA1
#define REC_DMA     DMA1
#define REC_DMA_CHANNEL DMA_CHANNEL1
#define REC_DMA_ISR     NVIC_DMA1_CHANNEL1_IRQ

#define REC_RCC_ADC RCC_ADC1
#define REC_ADC     ADC1
#define REC_ADC_TSAMPLE ADC_SMPR_SMP_71DOT5CYC

#define L_REC_ADC   ADC_CHANNEL10
#define LF_REC_ADC  ADC_CHANNEL11
#define RF_REC_ADC  ADC_CHANNEL12
#define R_REC_ADC   ADC_CHANNEL13

void ADC_init(void);

#endif
