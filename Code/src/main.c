#include "mouse.h"

/*****************************************************************************/
// Global Variables
/*****************************************************************************/
__IO uint16_t sensor_buffers[4];
__IO uint16_t sensor_readings[4];

/*****************************************************************************/
// Main Function
/*****************************************************************************/
int main(void)
{
  // MCU Configurations
  RCC_Configuration();
  GPIO_Configuration();
  DMA_Configuration();
  NVIC_Configuration();

  ADC_Configuration();
  PWM_Configuration();
  ENCODER_Configuration();
  /*IIC_Configuration();*/
  /*MPU_Configuration();*/

  // Only for debug
  USART_Configuration();


  mouse_state = STOP;

  /*turnMotorOn();*/
  while(1)
  {
    // Listen to button push
    listen_for_button();

    switch(mouse_state) {
      case GO:
        turnMotorOn();
        // Do PID
        pid();
        /*GPIO_SetBits(GPIOC, GREEN);*/
        /*GPIO_ResetBits(GPIOB, RED);*/
        /*GPIO_ResetBits(GPIOC, YELLOW);*/
        break;

      case TEST:
        //turnMotorOn();
        //change_LeftMotorSpeed(120);
        //change_RightMotorSpeed(120);
        /*leftTurn();*/
        rightTurn();
        Delay_us(1000000);
        GPIO_SetBits(GPIOC, YELLOW);
        GPIO_SetBits(GPIOC, GREEN);
        GPIO_ResetBits(GPIOB, RED);
        break;

      case STOP:
        /*ADC_Read();*/

        turnMotorOff();
        GPIO_SetBits(GPIOB, RED);
        GPIO_ResetBits(GPIOC, GREEN);
        GPIO_SetBits(GPIOC, YELLOW);
        break;
    }
    
  
    /*printf("%u            %u\r\n", TIM3->CCR3, TIM4->CCR4);*/

            /*sensor_readings[0], sensor_readings[1], sensor_readings[2], sensor_readings[3]);*/
            /*ADC1->JDR1, ADC1->JDR2, ADC1->JDR3, ADC1->JDR4);*/
    /*printf("                                              sensor reading: %u         %u        %u        %u\r\n",*/
            /*sensor_buffers[0], sensor_buffers[1], sensor_buffers[2], sensor_buffers[3]);*/
            /*ADC1->JOFR1, ADC1->JOFR2, ADC1->JOFR3, ADC1->JOFR4);*/
    /*printf("%u            %u\r\n", L_ENC->CNT, R_ENC->CNT);*/
    /*if(TIM8->CNT > 7000)*/
        /*turnMotorOff();*/
  
  }
}
