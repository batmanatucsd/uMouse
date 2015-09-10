#ifndef _MCU_ADC_H_
#define _MCU_ADC_H_

#include <stdint.h>

#define REC_RCC_PORT    RCC_GPIOC
#define REC_PORT        GPIOC

#define L_REC_PIN   GPIO0
#define LF_REC_PIN  GPIO1
#define RF_REC_PIN  GPIO2
#define R_REC_PIN   GPIO3

#define REC_RCC_DMA RCC_DMA1
#define REC_DMA     DMA1
#define REC_DMA_CHANNEL 1

#define REC_RCC_ADC RCC_ADC1
#define REC_ADC ADC1

#define L_REC_ADC   ADC_Channel_10
#define LF_REC_ADC  ADC_Channel_11
#define RF_REC_ADC  ADC_Channel_12
#define R_REC_ADC   ADC_Channel_13

void ADC_init(void);

#endif
