#include "mcu_lib.h"

void delay(volatile int);

int main(void)
{
	USART_Configuration();

	while(1)
	{
		USART_SendData(USART1, 'd');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		{}
	}
	return 0;
}

void delay(volatile int i)
{
	while(i--);
}
