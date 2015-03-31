#include "mouse.h"
#include "mcu_lib.h"

/*****************************************************************************/
// RCC_Configuration
//
// @brief: Configure clocks for the peripherals
/*****************************************************************************/
void RCC_Configuration(void) /*{{{*/
{
	/* PCLK2 is the APB2 clock */
  /* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
  // RCC_ADCCLKConfig(RCC_PCLK2_Div6);

  //enable DMA1 clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

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
// @brief: Set general purpose input & output pin configurations
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

/*****************************************************************************/
// DMA_Configuration
//
// @brief: Set configurations for DMA
/*****************************************************************************/
void DMA_Configuration(void) /*{{{*/
{
  //create DMA structure
  DMA_InitTypeDef  DMA_InitStructure;

  //reset DMA1 channe1 to default values;
  DMA_DeInit(DMA1_Channel1);

  //channel will be used for memory to memory transfer
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  //setting normal mode (non circular)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  //medium priority
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;

  //chunk of data to be transfered
  DMA_InitStructure.DMA_BufferSize = 4; // use 4 when using 4 IR sensors
  //source and destination data size word=32bit
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

  //Location assigned to peripheral register will be source
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

  //source and destination start addresses
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  /*DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)sensor_readings;*/
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC1->JOFR1;

  //source address increment disable
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  //automatic memory destination increment enable.
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

  //send values to DMA registers
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  // Enable DMA1 Channel Transfer Complete interrupt
  /*DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);*/
  DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1
}/*}}}*/

/*****************************************************************************/
// ADC_Configuration
//
// @brief: Set configurations for analog input
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
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  /* Don't do contimuous conversions - do them on demand */
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

  // Start conversin by software, not an external trigger
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  /* Conversions are 12 bit - put them in the lower 12 bits of the result */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  /* Say how many channels would be used by the sequencer */
  ADC_InitStructure.ADC_NbrOfChannel = 4;

  ADC_RegularChannelConfig(ADC1, L_RECEIVER, 1, ADC_SampleTime_41Cycles5);
  ADC_RegularChannelConfig(ADC1, R_RECEIVER, 2, ADC_SampleTime_41Cycles5);
  ADC_RegularChannelConfig(ADC1, LF_RECEIVER, 3, ADC_SampleTime_41Cycles5);
  ADC_RegularChannelConfig(ADC1, RF_RECEIVER, 4, ADC_SampleTime_41Cycles5);

  /* Set injected sequencer length */
  ADC_InjectedSequencerLengthConfig(ADC1, 4);
  /* ADC1 injected channel Configuration */ 
  ADC_InjectedChannelConfig(ADC1, L_RECEIVER, 1, ADC_SampleTime_41Cycles5);
  ADC_InjectedChannelConfig(ADC1, R_RECEIVER, 2, ADC_SampleTime_41Cycles5);
  ADC_InjectedChannelConfig(ADC1, LF_RECEIVER, 3, ADC_SampleTime_41Cycles5);
  ADC_InjectedChannelConfig(ADC1, RF_RECEIVER, 4, ADC_SampleTime_41Cycles5);
  /* ADC1 injected external trigger configuration */
  ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);

  /* Enable ADC1 Injected Channels in discontinous mode */
  ADC_InjectedDiscModeCmd(ADC1, ENABLE);

  /* Enable ADC2 EOC interrupt */
  ADC_ITConfig(ADC2, ADC_IT_JEOC, ENABLE);

  /* Now do the setup */
  ADC_Init(ADC1, &ADC_InitStructure);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /*Enable DMA for ADC*/
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Start the conversion */
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}/*}}}*/

/*****************************************************************************/
// ADC_Read
//
// @brief: Gets analog input
// @param: channel : adc_channel to get the result from
/*****************************************************************************/
uint16_t ADC_Read(uint8_t channel, int rank) /*{{{*/
{
  /*ADC_RegularChannelConfig(ADC1, channel, rank, ADC_SampleTime_7Cycles5);*/
  // Start the conversion
  /*ADC_SoftwareStartConvCmd(ADC1, ENABLE);*/
  /*Wait until conversion completion*/
  /*while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);*/
  /*Get the conversion value*/
  /*return ADC_GetConversionValue(ADC1);*/
  ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);
  (ADC_GetFlagStatus(ADC1, ADC_FLAG_JEOC) == RESET);
  return;
}/*}}}*/

/*****************************************************************************/
// PWM_Configuration
//
// @brief: Set configurations for PWM
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

/*****************************************************************************/
// Delay_Init
//
// @brief: Set configurations for delay functions
// @param: SYSCLK : System Clock
/*****************************************************************************/
void Delay_Init() //SYSCLK1000000??72MHz?72  /*{{{*/
{  
    SysTick->CTRL&=0xFFFFFFFB;  
    /*FAC_US=SYSCLK/8;  */
    /*FAC_MS=(u16t)(FAC_US*1000);       */
}  /*}}}*/

/*****************************************************************************/
// Delay_us
//
// @brief: Delay for given amount of microseconds
// @param: nus : microseconds
/*****************************************************************************/
void Delay_us(uint32_t nus)  /*{{{*/
{  
    uint32_t temp;  
    SysTick->LOAD=nus*FAC_US;  
    SysTick->VAL=0x00;  
    SysTick->CTRL=0x01;  
    do  
    {  
        temp=SysTick->CTRL;  
    }  
    while(temp&0x01&&!(temp&0x10000));  
    SysTick->CTRL=0x00;  
    SysTick->VAL=0x00;     
}  /*}}}*/

/*****************************************************************************/
// Delay_ms
//
// @brief: Delay for given amount of miliseconds
// @param: nms : milliseconds to delay
/*****************************************************************************/
void Delay_ms(uint16_t nms)  /*{{{*/
{  
    uint32_t temp;  
    SysTick->LOAD=(uint32_t)(nms*FAC_MS);  
    SysTick->VAL=0x00;  
    SysTick->CTRL=0x01;  
    do  
    {  
        temp=SysTick->CTRL;  
    }  
    while(temp&0x01&&!(temp&0x10000));  
    SysTick->CTRL=0x00;  
    SysTick->VAL=0x00;     
}  /*}}}*/
