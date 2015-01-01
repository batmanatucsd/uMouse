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
  // ADC_Configuration();
  PWM_Configuration();

  // Only for debug
  // USART_Configuration();

  while(1)                                                                      
  {                                                                             
    /*
    // USART debugging
		USART_SendData(USART1, 'd');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}
    */

    //GPIO_SetBits(GPIOC, GPIO_Pin_8);                                          
    GPIO_SetBits(GPIOC, GPIO_Pin_9);                                            
    delay(10000);                                                             
 
    //GPIO_ResetBits(GPIOC, GPIO_Pin_8);                                        
    GPIO_ResetBits(GPIOC, GPIO_Pin_9);                                          
    
    delay(1000000);                                                             
  }                  
	return 0;
}

/*****************************************************************************/
// Function Definitions
/*****************************************************************************/
void delay(volatile int i)
{
	while(i--);
}
