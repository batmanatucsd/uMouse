#include "mouse.h"

/*****************************************************************************/
// Function Declarations
/*****************************************************************************/
void delay(volatile int);

/*****************************************************************************/
// Main Function
/*****************************************************************************/
int main(void)
{

  volatile uint16_t sensorReading = 0;
  RCC_Configuration();
  GPIO_Configuration();
  ADC_Configuration();
  /*PWM_Configuration();*/
  /*IIC_Configuration();*/
  /*MPU_Configuration();*/

  // Only for debug
  USART_Configuration();
  /*uint8_t test =0;*/

  /*delay(100);*/
  /*USART_Write(0x03);*/

//  Commented portion/*{{{*/
//  while(1)
//  {
//    /*
//    // USART debugging
//    USART_Write('d');
//    */

//    //GPIO_SetBits(GPIOC, GPIO_Pin_8);
//    GPIO_SetBits(GPIOC, GPIO_Pin_9);
//    delay(10000);

//    //GPIO_ResetBits(GPIOC, GPIO_Pin_8);
//    GPIO_ResetBits(GPIOC, GPIO_Pin_9);

//    delay(1000000);
//  }
// return 0;/*}}}*/
  while(1)
  {
    // Testing PWM
    GPIO_SetBits(GPIOC, GPIO_Pin_9);
    /*GPIO_SetBits(GPIOC, GPIO_Pin_4);*/
    /*GPIO_SetBits(GPIOB, GPIO_Pin_0);*/
    /*GPIO_ResetBits(GPIOC, GPIO_Pin_5);*/
    delay(1000000);

    GPIO_ResetBits(GPIOC, GPIO_Pin_9);
    /*GPIO_ResetBits(GPIOC, GPIO_Pin_4);*/
    /*GPIO_ResetBits(GPIOB, GPIO_Pin_0);*/
    /*GPIO_SetBits(GPIOC, GPIO_Pin_5);*/
    delay(1000000);

    sensorReading = readADC(ONESENSOR);
    if(sensorReading > 200)
      USART_Write('A');
    else if(sensorReading > 190)
      USART_Write('B');
    else if(sensorReading > 180)
      USART_Write('C');
    else if(sensorReading > 170)
      USART_Write('D');
    else if(sensorReading > 160)
      USART_Write('E');
    else if(sensorReading > 150)
      USART_Write('F');
    else if(sensorReading > 140)
      USART_Write('a');
    else if(sensorReading > 130)
      USART_Write('b');
    else if(sensorReading > 120)
      USART_Write('c');
    else if(sensorReading > 110)
      USART_Write('d');
    else if(sensorReading > 100)
      USART_Write('e');
    else 
      USART_Write('f');

    USART_Write(' ');

    /*test = IIC_ReadDeviceRegister(MPU_ADDR,DEVID);*/
    /*USART_Write(test);*/

    /*test = MPU_ReadID();*/
    /*USART_Write(test);*/

    //IIC_DMA_Read(MPU_ADDR,DEVID);

    /*USART_Write('d');*/

    //while(IIC_RX_OUTPUT == 0){}

    //for(temp = 0; temp < 6; temp++)
    //{
    //	USART_Write(IIC_RX_Buffer[temp]);
    //}
  }
}

/*****************************************************************************/
// Function Definitions
/*****************************************************************************/
void delay(volatile int i)
{
	while(i--);
}
