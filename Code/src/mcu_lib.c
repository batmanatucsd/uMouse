#include <stdio.h>
#include "mcu_lib.h"

/*****************************************************************************/
// RCC_Configuration
//
// Configure clocks for the peripherals
/*****************************************************************************/
void RCC_Configuration(void) /*{{{*/
{
	/* PCLK2 is the APB2 clock */
  /* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
  // RCC_ADCCLKConfig(RCC_PCLK2_Div6);

  /* ADCCLK = PCLK2/4 for ADC */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4);

   /* TIM clock enable for PWM */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

  // TODO: TIM clock enable for encoder counts

  /* GPIO, ADC clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO |
                         RCC_APB2Periph_ADC1, ENABLE);
}/*}}}*/

/*****************************************************************************/
// GPIO_Configuration
//
// Set general purpose input & output pin configurations
/*****************************************************************************/
void GPIO_Configuration(void) /*{{{*/
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// ***** For LED's ***** //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ***** For Push Button ***** //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ***** For IR Sensors ***** //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                                GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ***** For Motor Controls ***** //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ***** For PWM output ***** //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ***** For Encoder Inputs ***** //
  // TODO: Configure PB6,7 for RIGHT
  // TODO: Configure PC6,7 for LEFT
}/*}}}*/

#ifdef SERIAL_DEBUG
/*****************************************************************************/
// USART_Configuration
//
// Set configurations for serial connection
// Only called when in debug mode
/*****************************************************************************/
void USART_Configuration(void) /*{{{*/
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

	// **** GPIO config for serial connection *** //
  // USART_RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // USART_TX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}/*}}}*/

void USART_Write(uint16_t Data) /*{{{*/
{
  USART_SendData(USART1, Data);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}/*}}}*/

uint8_t USART_Read(void) {/*{{{*/
  while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
  return (uint8_t) USART_ReceiveData(USART1);
}/*}}}*/

void USART_SendInt(uint16_t num) /*{{{*/
{
  int i = 5;
  char num_char[6] = {0};

  while(num > 0) {
    num_char[i--] = num%10 + 48;
    num /= 10;
  }

  for(i=0; i<6; ++i) {
    if(num_char[i])
      USART_Write(num_char[i]);
  }
}/*}}}*/

#endif // SERIAL_DEBUG

/*****************************************************************************/
// ADC_Configuration
//
// Set configurations for analog input
/*****************************************************************************/
void ADC_Configuration(void) /*{{{*/
{
  ADC_InitTypeDef  ADC_InitStructure;

  /* Put everything back to power-on defaults */
  ADC_DeInit(ADC1);

  /* ADC1 Configuration ------------------------------------------------------*/
  /* ADC1 and ADC2 operate independently */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  /* Disable the scan conversion so we do one at a time */
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  /* Don't do contimuous conversions - do them on demand */
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  // Start conversin by software, not an external trigger
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  /* Conversions are 12 bit - put them in the lower 12 bits of the result */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  /* Say how many channels would be used by the sequencer */
  ADC_InitStructure.ADC_NbrOfChannel = 1;

  /* Now do the setup */
  ADC_Init(ADC1, &ADC_InitStructure);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
}/*}}}*/

/*****************************************************************************/
// readADC
//
// Gets analog input
/*****************************************************************************/
uint16_t ADC_Read(uint8_t channel) /*{{{*/
{
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);
  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  // Wait until conversion completion
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  // Get the conversion value
  return ADC_GetConversionValue(ADC1);
}/*}}}*/

/*****************************************************************************/
// PWM_Configuration
//
// Set configurations for PWM
/*****************************************************************************/
void PWM_Configuration(void) /*{{{*/
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

	// Compute the prescaler value
  uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;

  // Time base configuration
  TIM_TimeBaseStructure.TIM_Period = 750;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); // Initialize TIM3
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); // Initialize TIM5

  // Channel Configuration
  // Mode: PWM1
  // Polarity: HIGH
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  // TIM 5 Channel2 for RIGHT Motor
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 125;

  TIM_OC2Init(TIM5, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);

  // TODO: might need to change the TIMer
  // TIM3 Channel2 for LEFT Motor
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 100;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM5, ENABLE);
  TIM_ARRPreloadConfig(TIM3, ENABLE);

  // TIM3,5 enable counter
  TIM_Cmd(TIM5, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
}/*}}}*/
