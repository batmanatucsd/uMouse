#include <stm32f10x.h>

GPIO_InitTypeDef GPIO_InitStructure;

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	/* Configure PD0 and PD2 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	while(1)
	{
		/* Set PD0 and PD2 */
		GPIOD->BSRR = 0x00000005;
		/* Reset PD0 and PD2 */
		GPIOD->BRR  = 0x00000005;
	}
	return 0;
}
