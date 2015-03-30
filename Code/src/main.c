#include "mouse.h"
#include <stdio.h>
#include "usart.h"

/*****************************************************************************/
// Main Function
/*****************************************************************************/
int main(void)
{
  volatile uint16_t sensorReading = 0;

  // MCU Configurations
  RCC_Configuration();
  GPIO_Configuration();
  ADC_Configuration();
  PWM_Configuration();
  /*IIC_Configuration();*/
  /*MPU_Configuration();*/

  // Only for debug
  USART_Configuration();

  mouse_state = STOP;

  while(1)
  {
    // Listen to button push
    listen_for_button();

    // Do PID
    
    switch(mouse_state) {
      case GO:
        GPIO_ResetBits(GPIOB, RED);
        GPIO_SetBits(GPIOC, GREEN);
        turnMotorOn();
        break;
      case STOP:
        GPIO_SetBits(GPIOB, RED);
        GPIO_ResetBits(GPIOC, GREEN);
        turnMotorOff();
        break;
    }
    
    // IR Sensors
    GPIO_SetBits(EMITTER, L_EMITTER);
    sensorReading = ADC_Read(L_RECEIVER);

    printf("sensor reading: %d\r\n", sensorReading);
  }
}
