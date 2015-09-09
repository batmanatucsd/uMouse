#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
//#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/usart.h>
#include "mcu_usart.h"

static void clock_setup(void)
{
	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_USART1);
}

static void usart_setup(void)
{
	/* Setup GPIO pin GPIO_USART1_TX(9) on GPIO port A for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup GPIO pin GPIO_USART1_RX(10) on GPIO port A for receive. */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 9600);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

int _write(int file, char *ptr, int len)
{
	cm_disable_interrupts();
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(USART1, ptr[i]);
		return i;
	}

	errno = EIO;
	cm_enable_interrupts();
	return -1;
}

// int _read(int file, char *ptr, int len)
// {
//
// }

//confiure usart
void USART_init(void)
{
  clock_setup();
  usart_setup();
}

//Unit test
// int main(void)
// {
// 	int counter = 0;
// 	float fcounter = 0.0;
// 	double dcounter = 0.0;
//
//   USART_Configuration();
//
// 	/*
// 	 * Write Hello World, an integer, float and double all over
// 	 * again while incrementing the numbers.
// 	 */
// 	while (1) {
// 		printf("Hello World! %i %f %f\r\n", counter, fcounter,
// 		       dcounter);
// 		counter++;
// 		fcounter += 0.01;
// 		dcounter += 0.01;
// 	}
//
// 	return 0;
// }
