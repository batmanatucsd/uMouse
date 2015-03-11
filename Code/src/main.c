#include "mcu_lib.h"

/*****************************************************************************/
// Function Declarations
/*****************************************************************************/
void delay(volatile int);

/*****************************************************************************/
// Main Function
/*****************************************************************************/
int main(void)
{
	//RCC_Configuration();
	//GPIO_Configuration();
  // ADC_Configuration();
  //PWM_Configuration();
  MPU_Configuration();

  // Only for debug
  USART_Configuration();
  uint8_t test =0;
	//uint16_t data[3];

  //  Commented portion
	//  while(1)                                                                      /*{{{*/
	//  {                                                                             /*{{{*/
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
	//  }                  /*}}}*/
	// return 0;/*}}}*/

	delay(100);
  USART_Write(0x03);
  while(1)
  {
		test = MPU_ReadID();
		USART_Write(test);

		//IIC_DMA_Read(MPU_ADDR,DEVID);

		//USART_Write(0x03);

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
