#include "mouse.h"

/*****************************************************************************/
// Main Function
/*****************************************************************************/
int main(void)
{

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
  

    ADC_Read();

    printf("ADC1 reading: %u         %u        %u        %u\r\n",
            ADC3->JOFR1, ADC3->JOFR2, ADC3->JOFR3, ADC3->JOFR4);
    printf("                                              sensor reading: %u         %u        %u        %u\r\n",
            sensor_readings[0], sensor_readings[1], sensor_readings[2], sensor_readings[3]);
            /*ADC3->DR, sensor_readings[1], sensor_readings[2], sensor_readings[3]);*/
  
  }
}
