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

  while(1)
  {
    // Testing PWM
    GPIO_SetBits(GPIOC, GPIO_Pin_9);
    delay(1000000);

    GPIO_ResetBits(GPIOC, GPIO_Pin_9);
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
  }
}

/*****************************************************************************/
// Function Definitions
/*****************************************************************************/
void delay(volatile int i)
{
	while(i--);
}
