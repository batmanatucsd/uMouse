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

  RCC_Configuration();
	GPIO_Configuration();
  //ADC_Configuration();
  PWM_Configuration();
  /*IIC_Configuration();*/

  // Only for debug
  USART_Configuration();
  /*uint8_t test =0;*/

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
    /*USART_Write('a');*/

    /*test = IIC_ReadDeviceRegister(MPU_ADDR,DEVID);*/
    /*USART_Write(test);*/
   
    TIM3->CCR3 = PWIDTH_MAX;
    GPIO_SetBits(GPIOC, GPIO_Pin_9);
    delay(1000000);

    TIM3->CCR3 = PWIDTH_0;
    GPIO_ResetBits(GPIOC, GPIO_Pin_9);
    delay(1000000);
  }
}

/*****************************************************************************/
// Function Definitions
/*****************************************************************************/
void delay(volatile int i)
{
	while(i--);
}
