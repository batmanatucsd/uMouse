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
  ADC_Configuration();
  PWM_Configuration();
  /*IIC_Configuration();*/
  /*MPU_Configuration();*/

  // Only for debug
  USART_Configuration();

  mouse_state = GO;

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
        GPIO_ResetBits(GPIOC, GREEN);
        /*turnMotorOff();*/
        break;
    }
    
    // IR Sensors
    GPIO_SetBits(EMITTER, E_LEFT);
    sensorReading = ADC_Read(PT_LEFT);

    /*USART_SendInt(sensorReading);*/
    /*USART_Write(' ');*/
    /* IR_Sensor Debug *//*{{{*/
    /*sensorReading = readADC(ONESENSOR);*/
    /*if(sensorReading > 200)*/
      /*USART_Write('A');*/
    /*else if(sensorReading > 190)*/
      /*USART_Write('B');*/
    /*else if(sensorReading > 180)*/
      /*USART_Write('C');*/
    /*else if(sensorReading > 170)*/
      /*USART_Write('D');*/
    /*else if(sensorReading > 160)*/
      /*USART_Write('E');*/
    /*else if(sensorReading > 150)*/
      /*USART_Write('F');*/
    /*else if(sensorReading > 140)*/
      /*USART_Write('a');*/
    /*else if(sensorReading > 130)*/
      /*USART_Write('b');*/
    /*else if(sensorReading > 120)*/
      /*USART_Write('c');*/
    /*else if(sensorReading > 110)*/
      /*USART_Write('d');*/
    /*else if(sensorReading > 100)*/
      /*USART_Write('e');*/
    /*else */
      /*USART_Write('f');*/

    /*USART_Write(' ');*//*}}}*/
  }
}
