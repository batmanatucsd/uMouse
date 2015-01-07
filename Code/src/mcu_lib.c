#include "mcu_lib.h"

// TODO:
// Don't forget to change the pin numbers

/*****************************************************************************/
// RCC_Configuration
//
// Configure clocks for the peripherals
/*****************************************************************************/
void RCC_Configuration(void)
{
	/* PCLK2 is the APB2 clock */
  /* ADCCLK = PCLK2/6 = 72/6 = 12MHz*/
  // RCC_ADCCLKConfig(RCC_PCLK2_Div6);

  /* ADCCLK = PCLK2/4 for ADC */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 

   /* TIM clock enable for PWM */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIO, ADC clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
   RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO |
   RCC_APB2Periph_ADC1, ENABLE);
}

/*****************************************************************************/
// GPIO_Configuration
//
// Set general purpose input & output pin configurations
/*****************************************************************************/
void GPIO_Configuration(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// **** GPIO config for digital output push-pull *** //
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// **** GPIO config for analog input *** //
	/*
	void GPIO_Configuration(void)
	{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  // Configure PC.02, PC.03 and PC.04 (ADC Channel12, ADC Channel13 and 
	  //   ADC Channel14) as analog inputs 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	}
	*/

  // GPIOA Configuration:TIM3 Channel1, 2, 3 and 4 as alternate function push-pull 
  // TIM3
  // Channel 1 : PA6
  // Channel 2 : PA7
  // Channel 3 : PB0
  // Channel 4 : PB1
  //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

#ifdef  SERIAL_DEBUG
/*****************************************************************************/
// USART_Configuration
// 
// Set configurations for serial connection
// Only called when in debug mode
/*****************************************************************************/
void USART_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
  GPIO_InitTypeDef GPIO_InitStructure;
  
	// **** GPIO config for serial connection *** //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

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
}

void USART_Write(uint16_t Data)
{
  USART_SendData(USART1, Data);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
}

#endif // SERIAL_DEBUG

/*****************************************************************************/
// ADC_Configuration
//
// Set configurations for analog input
// ADC on PC0, PC1, PC2, PC3
// ADC123_IN10, 11, 12 ,13
/*****************************************************************************/
void ADC_Configuration(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

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
}

/*****************************************************************************/
// PWM_Configuration
//
// Set configurations for PWM
/*****************************************************************************/
void PWM_Configuration(void)
{
	// Compute the prescaler value 
  uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
  
  // Time base configuration 
  TIM_TimeBaseStructure.TIM_Period = 665;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


  // PWM1 Mode configuration: Channel3 
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = PWIDTH_MAX;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);


  // PWM1 Mode configuration: Channel4 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = PWIDTH_0;

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);


  TIM_ARRPreloadConfig(TIM3, ENABLE);

  // TIM3 enable counter 
  TIM_Cmd(TIM3, ENABLE);
}

/*****************************************************************************/
// MPU_Configuration
//
// Set configurations for MPU6050(debug for adxl345 right now)
/*****************************************************************************/
void MPU_Configuration(void)
{
  /* Enable I2C and I2C_PORT & Alternate Function clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  
  /* Reset I2C IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
  
  /* Release reset signal of I2C IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
  
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  I2C_InitTypeDef I2C_InitStructure;
  
  I2C_InitStructure.I2C_ClockSpeed = 300000;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  
  I2C_Init(I2C1, &I2C_InitStructure);

  I2C_Cmd(I2C1, ENABLE);
}

void init_sensor(void)
{
  /* initiate start sequence */
  I2C_GenerateSTART(I2C1, ENABLE);
  /* check start bit flag */
  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB));
  /*send write command to chip*/
  I2C_Send7bitAddress(I2C1, MPU_ADDR, I2C_Direction_Transmitter);
  /*check master is now in Tx mode*/
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  /*mode register address*/
  I2C_SendData(I2C1, 0x02);
  /*wait for byte send to complete*/
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  /*clear bits*/
  I2C_SendData(I2C1, 0x00);
  /*wait for byte send to complete*/
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  /*generate stop*/
  I2C_GenerateSTOP(I2C1, ENABLE);
  /*stop bit flag*/
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
}

uint8_t I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr)
{
  uint8_t TEMP;
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  while (I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
  /**********************************************/
  I2C_GenerateSTART(I2C1, ENABLE);

  while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB));

  I2C_Send7bitAddress(I2C1, DeviceAddr, I2C_Direction_Transmitter);

  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C1, RegisterAddr);

  while (!I2C_GetFlagStatus(I2C1,I2C_FLAG_TXE));
  /**********************************************/
  I2C_GenerateSTART(I2C1, ENABLE);
  
  while (!I2C_GetFlagStatus(I2C1,I2C_FLAG_SB));
  
  I2C_Send7bitAddress(I2C1, DeviceAddr, I2C_Direction_Receiver);
    
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
  TEMP = I2C_ReceiveData(I2C1);
  /**********************************************/
  I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);
  I2C_AcknowledgeConfig(I2C1, DISABLE);

  I2C_GenerateSTOP(I2C1, ENABLE);
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));

  return TEMP;
}
