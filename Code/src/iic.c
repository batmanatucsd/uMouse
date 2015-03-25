#include "iic.h"
#include "mcu_lib.h"

/**
  * @brief  Writes a value in a register of the device through I2C.
  * @param  DeviceAddr: The address of the IICxpander, could be : IIC_1_ADDR.
  * @param  RegisterAddr: The target register adress
  * @param  RegisterValue: The target register value to be written
  * @retval IIC_OK: if all operations are OK. Other value if error.
  */
uint8_t I2C_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue)
{
  uint32_t read_verif = 0;
  uint8_t IIC_BufferTX = 0;

  /* Get Value to be written */
  IIC_BufferTX = RegisterValue;

  /* Configure DMA Peripheral */
  IIC_DMA_Config(IIC_DMA_TX, (uint8_t*)(&IIC_BufferTX));

  /* Enable the I2C peripheral */
  I2C_GenerateSTART(IIC, ENABLE);

  /* Test on SB Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (I2C_GetFlagStatus(IIC,I2C_FLAG_SB) == RESET)
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IIC, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on ADDR Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!I2C_CheckEvent(IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Transmit the first address for r/w operations */
  I2C_SendData(IIC, RegisterAddr);

  /* Test on TXE FLag (data dent) */
  //IIC_TimeOut = TIMEOUT_MAX;
  while ((!I2C_GetFlagStatus(IIC,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(IIC,I2C_FLAG_BTF)))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Enable I2C DMA request */
  I2C_DMACmd(IIC,ENABLE);

  /* Enable DMA TX Channel */
  DMA_Cmd(IIC_DMA_TX_CHANNEL, ENABLE);

  /* Wait until DMA Transfer Complete */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!DMA_GetFlagStatus(IIC_DMA_TX_TCFLAG))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Wait until BTF Flag is set before generating STOP */
  //IIC_TimeOut = 2 * TIMEOUT_MAX;
  while ((!I2C_GetFlagStatus(IIC,I2C_FLAG_BTF)))
  {
  }

  /* Send STOP Condition */
  I2C_GenerateSTOP(IIC, ENABLE);

  /* Disable DMA TX Channel */
  DMA_Cmd(IIC_DMA_TX_CHANNEL, DISABLE);

  /* Disable I2C DMA request */
  I2C_DMACmd(IIC,DISABLE);

  /* Clear DMA TX Transfer Complete Flag */
  DMA_ClearFlag(IIC_DMA_TX_TCFLAG);

#ifdef VERIFY_WRITTENDATA
  /* Verify (if needed) that the loaded data is correct  */

  /* Read the just written register*/
  read_verif = I2C_ReadDeviceRegister(DeviceAddr, RegisterAddr);
  /* Load the register and verify its value  */
  if (read_verif != RegisterValue)
  {
    /* Control data wrongly tranfered */
    read_verif = IIC_FAILURE;
  }
  else
  {
    /* Control data correctly transfered */
    read_verif = 0;
  }
#endif

  /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return read_verif;
}

/**
  * @brief  Reads a register of the device through I2C.
  * @param  DeviceAddr: The address of the device, could be : IIC_1_ADDR.
  * @param  RegisterAddr: The target register adress (between 00x and 0x24)
  * @retval The value of the read register (0xAA if Timout occured)
  */
uint8_t I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr)
{
  uint8_t IIC_BufferRX[2] = {0x00, 0x00};

  /* Configure DMA Peripheral */
  IIC_DMA_Config(IIC_DMA_RX, (uint8_t*)IIC_BufferRX);

  /* Enable DMA NACK automatic generation */
  I2C_DMALastTransferCmd(IIC, ENABLE);

  /* Enable the I2C peripheral */
  I2C_GenerateSTART(IIC, ENABLE);

  /* Test on SB Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IIC,I2C_FLAG_SB))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send device address for write */
  I2C_Send7bitAddress(IIC, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on ADDR Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!I2C_CheckEvent(IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send the device's internal address to write to */
  I2C_SendData(IIC, RegisterAddr);

  /* Test on TXE FLag (data dent) */
  //IIC_TimeOut = TIMEOUT_MAX;
  while ((!I2C_GetFlagStatus(IIC,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(IIC,I2C_FLAG_BTF)))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send START condition a second time */
  I2C_GenerateSTART(IIC, ENABLE);

  /* Test on SB Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IIC,I2C_FLAG_SB))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send IICxpander address for read */
  I2C_Send7bitAddress(IIC, DeviceAddr, I2C_Direction_Receiver);

  /* Test on ADDR Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!I2C_CheckEvent(IIC, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Enable I2C DMA request */
  I2C_DMACmd(IIC,ENABLE);

  /* Enable DMA RX Channel */
  DMA_Cmd(IIC_DMA_RX_CHANNEL, ENABLE);

  /* Wait until DMA Transfer Complete */
  //IIC_TimeOut = 2 * TIMEOUT_MAX;
  while (!DMA_GetFlagStatus(IIC_DMA_RX_TCFLAG))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send STOP Condition */
  I2C_GenerateSTOP(IIC, ENABLE);

  /* Disable DMA RX Channel */
  DMA_Cmd(IIC_DMA_RX_CHANNEL, DISABLE);

  /* Disable I2C DMA request */
  I2C_DMACmd(IIC,DISABLE);

  /* Clear DMA RX Transfer Complete Flag */
  DMA_ClearFlag(IIC_DMA_RX_TCFLAG);

  /* return a pointer to the IIC_Buffer */
  return (uint8_t)IIC_BufferRX[0];
}


/**
  * @brief  Reads a buffer of 2 bytes from the device registers.
  * @param  DeviceAddr: The address of the device, could be : IIC_1_ADDR.
  * @param  RegisterAddr: The target register adress (between 00x and 0x24)
  * @retval A pointer to the buffer containing the two returned bytes (in halfword).
  */
uint16_t I2C_ReadDataBuffer(uint8_t DeviceAddr, uint32_t RegisterAddr)
{
  uint8_t tmp= 0;
  uint8_t IIC_BufferRX[2] = {0x00, 0x00};

  /* Configure DMA Peripheral */
  IIC_DMA_Config(IIC_DMA_RX, (uint8_t*)IIC_BufferRX);

  /* Enable DMA NACK automatic generation */
  I2C_DMALastTransferCmd(IIC, ENABLE);

  /* Enable the I2C peripheral */
  I2C_GenerateSTART(IIC, ENABLE);

  /* Test on SB Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IIC,I2C_FLAG_SB))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send device address for write */
  I2C_Send7bitAddress(IIC, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on ADDR Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!I2C_CheckEvent(IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send the device's internal address to write to */
  I2C_SendData(IIC, RegisterAddr);

  /* Test on TXE FLag (data dent) */
  //IIC_TimeOut = TIMEOUT_MAX;
  while ((!I2C_GetFlagStatus(IIC,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(IIC,I2C_FLAG_BTF)))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send START condition a second time */
  I2C_GenerateSTART(IIC, ENABLE);

  /* Test on SB Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IIC,I2C_FLAG_SB))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send IICxpander address for read */
  I2C_Send7bitAddress(IIC, DeviceAddr, I2C_Direction_Receiver);

  /* Test on ADDR Flag */
  //IIC_TimeOut = TIMEOUT_MAX;
  while (!I2C_CheckEvent(IIC, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Enable I2C DMA request */
  I2C_DMACmd(IIC,ENABLE);

  /* Enable DMA RX Channel */
  DMA_Cmd(IIC_DMA_RX_CHANNEL, ENABLE);

  /* Wait until DMA Transfer Complete */
  //IIC_TimeOut = 2 * TIMEOUT_MAX;
  while (!DMA_GetFlagStatus(IIC_DMA_RX_TCFLAG))
  {
    //if (IIC_TimeOut-- == 0) return(IIC_TimeoutUserCallback());
  }

  /* Send STOP Condition */
  I2C_GenerateSTOP(IIC, ENABLE);

  /* Disable DMA RX Channel */
  DMA_Cmd(IIC_DMA_RX_CHANNEL, DISABLE);

  /* Disable I2C DMA request */
  I2C_DMACmd(IIC,DISABLE);

  /* Clear DMA RX Transfer Complete Flag */
  DMA_ClearFlag(IIC_DMA_RX_TCFLAG);

  /* Reorganize received data */
  tmp = IIC_BufferRX[0];
  IIC_BufferRX[0] = IIC_BufferRX[1];
  IIC_BufferRX[1] = tmp;

  /* return a pointer to the IIC_Buffer */
  return *(uint16_t *)IIC_BufferRX;
}

/**
  * @brief  Initializes the GPIO pins used by the IO expander.
  * @param  None
  * @retval None
  */
void IIC_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable IIC and IIC_PORT & Alternate Function clocks */
  RCC_APB1PeriphClockCmd(IIC_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(IIC_SCL_GPIO_CLK | IIC_SDA_GPIO_CLK | IIC_IT_GPIO_CLK
                         | RCC_APB2Periph_AFIO, ENABLE);

#ifdef IIC_REMAP_EN
  GPIO_PinRemapConfig(IIC_REMAP, ENABLE);
#endif
  /* Reset IIC IP */
  RCC_APB1PeriphResetCmd(IIC_CLK, ENABLE);

  /* Release reset signal of IIC IP */
  RCC_APB1PeriphResetCmd(IIC_CLK, DISABLE);

  /* IIC SCL and SDA pins configuration */
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure);

  /* IIC SCL and SDA pins configuration */
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
  GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);

  /* Set EXTI pin as Input PullUp - IO_Expander_INT */
  GPIO_InitStructure.GPIO_Pin = IIC_IT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(IIC_IT_GPIO_PORT, &GPIO_InitStructure);

  /* Connect IO Expander IT line to EXTI line */
  GPIO_EXTILineConfig(IIC_IT_EXTI_PORT_SOURCE, IIC_IT_EXTI_PIN_SOURCE);
}


/**
  * @brief  Configure the I2C Peripheral used to communicate with IO_Expanders.
  * @param  None
  * @retval None
  */
void IIC_Config(void)
{
  I2C_InitTypeDef I2C_InitStructure;

  /* IIC configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = IIC_SPEED;

  I2C_Init(IIC, &I2C_InitStructure);
}


/**
  * @brief  Configure the DMA Peripheral used to handle communication via I2C.
  * @param  None
  * @retval None
  */

void IIC_DMA_Config(IIC_DMADirection_TypeDef Direction, uint8_t* buffer)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHBPeriphClockCmd(IIC_DMA_CLK, ENABLE);

  /* Initialize the DMA_PeripheralBaseAddr member */
  DMA_InitStructure.DMA_PeripheralBaseAddr = IIC_DR;
  /* Initialize the DMA_MemoryBaseAddr member */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;
   /* Initialize the DMA_PeripheralInc member */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  /* Initialize the DMA_MemoryInc member */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  /* Initialize the DMA_PeripheralDataSize member */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /* Initialize the DMA_MemoryDataSize member */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* Initialize the DMA_Mode member */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  /* Initialize the DMA_Priority member */
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  /* Initialize the DMA_M2M member */
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  /* If using DMA for Reception */
  if (Direction == IIC_DMA_RX)
  {
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = 2;

    DMA_DeInit(IIC_DMA_RX_CHANNEL);

    DMA_Init(IIC_DMA_RX_CHANNEL, &DMA_InitStructure);
  }
   /* If using DMA for Transmission */
  else if (Direction == IIC_DMA_TX)
  {
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = 1;

    DMA_DeInit(IIC_DMA_TX_CHANNEL);

    DMA_Init(IIC_DMA_TX_CHANNEL, &DMA_InitStructure);
  }
}

/**
  * @brief  Configures the IO expander Interrupt line and GPIO in EXTI mode.
  * @param  None
  * @retval None
  */
void IIC_EXTI_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;

  /* Enable Button GPIO clock */
  RCC_APB2PeriphClockCmd(IIC_IT_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

  /* Configure Button pin as input floating */
  GPIO_InitStructure.GPIO_Pin = IIC_IT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(IIC_IT_GPIO_PORT, &GPIO_InitStructure);

  /* Connect Button EXTI Line to Button GPIO Pin */
  GPIO_EXTILineConfig(IIC_IT_EXTI_PORT_SOURCE, IIC_IT_EXTI_PIN_SOURCE);

  /* Configure Button EXTI line */
  EXTI_InitStructure.EXTI_Line = IIC_IT_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set Button EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = IIC_IT_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
