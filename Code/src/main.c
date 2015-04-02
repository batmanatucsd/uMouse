#include "mouse.h"

/*****************************************************************************/
// Main Function
/*****************************************************************************/
int main(void)
{
  volatile uint16_t sensorReading = 0;

  // MCU Configurations
  RCC_Configuration();
  GPIO_Configuration();
  DMA_Configuration();
  /*NVIC_Configuration();*/

  ADC_Configuration();
  /*PWM_Configuration();*/
  /*IIC_Configuration();*/
  /*MPU_Configuration();*/

  // Only for debug
  USART_Configuration();
  Delay_Init();

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
        /*turnMotorOn();*/
        break;

      case STOP:
        GPIO_SetBits(GPIOB, RED);
        /*GPIO_ResetBits(GPIOC, GREEN);*/
        /*turnMotorOff();*/
        break;
    }
    
  // IR Sensors
  /*GPIO_SetBits(EMITTER, L_EMITTER);*/
  /*GPIO_SetBits(EMITTER, LF_EMITTER);*/
  /*GPIO_SetBits(EMITTER, RF_EMITTER);*/
  /*GPIO_SetBits(EMITTER, R_EMITTER);*/
  
  /*printf("sensor reading: %d         %d        %d        %d\r\n",*/
          /*sensor_readings[0], sensor_readings[1], sensor_readings[2], sensor_readings[3]);*/


  ADC_Read();
  printf("sensor reading: %d         %d        %d        %d\r\n",
          /*ADC1->JDR1, ADC1->JDR2, ADC1->JDR3, ADC1->JDR4);*/
          ADC1->JDR1, 0, 0, 0);
  /*printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$OFFSET reading: %d         %d        %d        %d\r\n",*/
          /*ADC1->JOFR1, ADC1->JOFR2, ADC1->JOFR3, ADC1->JOFR4);*/
  
  }
}
