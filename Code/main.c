#include <stm32f10x.h>
void delay(volatile int);

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	while(1)
	{
		//GPIO_SetBits(GPIOC, GPIO_Pin_8);
		//delay(10000);
		//GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		//delay(10000);

		delay(1000000);

		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		delay(1000000);


		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		delay(1000000);
			}
	return 0;
}




void delay(volatile int i)
{
	while(i--);
}