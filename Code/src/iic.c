#include "iic.h"
#include "mcu_lib.h"

/*****************************************************************************/
// Init MPU
//
// Set configurations for MPU6050(debug for adxl345 right now)
/*****************************************************************************/
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

/*****************************************************************************/
// IIC_Configuration
//
// Design to be an univerial I2C Function
// Must be used once before all I2C communication
// Current Function: enable I2C1 on PB8 and PB9 (Remap)
/*****************************************************************************/
void IIC_Configuration(void)
{
  /* Enable I2C and I2C_PORT clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

  /* Reset I2C*/
  //I2C_DeInit(I2C1);

  /* Reset I2C IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);

  /* Release reset signal of I2C IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

  /* Configure the GPIO on the corresponding pins */
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable the Remap */
  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

  /* Configure the I2C */
  I2C_InitTypeDef I2C_InitStructure;

  I2C_InitStructure.I2C_ClockSpeed = 300000;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  I2C_Init(I2C1, &I2C_InitStructure);

  #ifdef IIC_DMA_EN
    IIC_DMA_Configuration();
    IIC_NVIC_Configuration();
  #endif

  /*Enable I2C*/
  I2C_Cmd(I2C1, ENABLE);
}

/*****************************************************************************/
// IIC_DMA_Configuration
//
// Design to be an univerial IIC Function
// Current Function: Enable I2C RX DMA
// Prerequsite: NONE
/*****************************************************************************/
void IIC_DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  //Reset DMA1 Channel 7 to default, which is used for I2C RX
  DMA_DeInit(IIC_RX_DMA_Channel);

  //Specify the I2C address
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)IIC_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)IIC_RX_Buffer;

  //Specify whether DMA run once or serveral time
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

  //Specify the dma priority
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

  //Specify the direction of dma
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

  //Specify whether Peripheral and Memory will increse by itself
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

  //Specify the size
  DMA_InitStructure.DMA_BufferSize = IIC_RX_BUFF_SIZE;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

  DMA_Init(IIC_RX_DMA_Channel, &DMA_InitStructure);
}

/*****************************************************************************/
// IIC_NVIC_Configuration
//
// Design to be an univerial IIC Function
// Current Function: prepare I2C RX DMA NVIC
// Prerequsite: NONE
/*****************************************************************************/
void IIC_NVIC_Configuration(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn; //I2C1 RX connect to channel 7 of DMA1
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************************************/
// IIC_ReadDeviceRegister
//
// Design to be an univerial IIC Function
// Used to Read Register Value for One Slave Device
// Current Function: Read register value
/*****************************************************************************/
uint8_t IIC_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr)
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

/*****************************************************************************/
// IIC_DMA_Read
//
// Design to be an univerial IIC Function
// Used to Read Register Value for One Slave Device but through the DMA channel
// Greatly save CPU time
// Current Function: Read register value
// Prerequsite: NEED to enable DMA ans NVIC before use.
/*****************************************************************************/
void IIC_DMA_Read(uint8_t DeviceAddr, uint8_t RegisterAddr)
{
  DMA_Cmd(IIC_RX_DMA_Channel, ENABLE);

  I2C_AcknowledgeConfig(I2C1, ENABLE);

  USART_Write(0x01);

  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  /* Enable DMA NACK automatic generation */
  I2C_DMALastTransferCmd(I2C1, ENABLE);

  USART_Write(0x02);

  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MPU6050 address for write */
  I2C_Send7bitAddress(I2C1, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C1, ENABLE);

  /* Send the MPU6050's internal address to write to */
  I2C_SendData(I2C1, RegisterAddr);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STRAT condition a second time */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MPU6050 address for read */
  I2C_Send7bitAddress(I2C1, DeviceAddr, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  /* Start DMA to receive data from I2C */
  //DMA_Cmd(IIC_RX_DMA_Channel, ENABLE);
  I2C_DMACmd(I2C1, ENABLE);

}

void DMA1_Channel7_IRQHandler(void)
{
  if (DMA_GetFlagStatus(DMA1_FLAG_TC7))
  {
    /* Clear transmission complete flag */
    DMA_ClearFlag(DMA1_FLAG_TC7);

    I2C_DMACmd(I2C1, DISABLE);
    /* Send I2C1 STOP Condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
    /* Disable DMA channel*/
    DMA_Cmd(IIC_RX_DMA_Channel, DISABLE);


    USART_Write(IIC_RX_Buffer[0]);

    //Read Accel data from byte 0 to byte 2
    //for(i=0; i<3; i++)
    //AccelGyro[i]=((s16)((u16)I2C_Rx_Buffer[2*i] << 8) + I2C_Rx_Buffer[2*i+1]);
    //Skip byte 3 of temperature data
    //Read Gyro data from byte 4 to byte 6
    //for(i=4; i<7; i++)
    //AccelGyro[i-1]=((s16)((u16)I2C_Rx_Buffer[2*i] << 8) + I2C_Rx_Buffer[2*i+1]);
    }
}
