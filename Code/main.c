#include "mcu_lib.h"
#include <stdio.h>

void delay(volatile int);

int main(void)
{
	
	RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration();

 /* 
	while(1)
	{
		USART_SendData(USART1, 'd');
		// printf("this is a test");
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}

    GPIO_SetBits(GPIOC, GPIO_Pin_9);                                            
    delay(10000);                                                             
 
    GPIO_ResetBits(GPIOC, GPIO_Pin_9);                                          
    delay(1000000);                                                             
	}
	
	
*/

  while(1)                                                                      
  {                                                                             
    GPIO_SetBits(GPIOC, GPIO_Pin_8);                                          
    GPIO_SetBits(GPIOC, GPIO_Pin_9);                                            
    delay(10000);                                                             
 
    GPIO_ResetBits(GPIOC, GPIO_Pin_8);                                        
    GPIO_ResetBits(GPIOC, GPIO_Pin_9);                                          
    
    delay(1000000);                                                             
  }                  
	return 0;
}

void delay(volatile int i)
{
	while(i--);
}
