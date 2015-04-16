#include "mouse.h"
#define L_ENC TIM8
#define R_ENC TIM4

/*****************************************************************************/
// General Functions
/*****************************************************************************/
void delay(volatile int i)/*{{{*/
{
  while(i--);
}/*}}}*/

void listen_for_button(void)/*{{{*/
{
  if(GPIO_ReadInputDataBit(GPIOB, BUTTON) != Bit_RESET) {
    state  next_state;

    if(mouse_state == STOP) // if the mouse_state is the last state
      next_state = GO;      // change it to the first state
    else 
      next_state = ++mouse_state; // else next state

    do {
      mouse_state = next_state;
    } while(GPIO_ReadInputDataBit(GPIOB, BUTTON) != Bit_RESET);
  }
}/*}}}*/

/*****************************************************************************/
// Motor Controls
/*****************************************************************************/
void forward(void)/*{{{*/
{
  GPIO_SetBits(MOTOR, STBY);

  // LEFT Motor CCW
  GPIO_SetBits(MOTOR, LEFTIN1);
  GPIO_ResetBits(MOTOR, LEFTIN2);

  // RIGHT MOtor CW
  GPIO_ResetBits(MOTOR, RIGHTIN1);
  GPIO_SetBits(MOTOR, RIGHTIN2);
}/*}}}*/

void backward(void)/*{{{*/
{
  GPIO_SetBits(MOTOR, STBY);

  // LEFT Motor CCW
  GPIO_SetBits(MOTOR, LEFTIN2);
  GPIO_ResetBits(MOTOR, LEFTIN1);

  // RIGHT MOtor CW
  GPIO_ResetBits(MOTOR, RIGHTIN2);
  GPIO_SetBits(MOTOR, RIGHTIN1);
}/*}}}*/

void turnMotorOff(void)/*{{{*/
{
  // TODO: should we change the way we turn off the motors?
  // perhaps turn the inputs off or the pwm (might save battery time)
  GPIO_WriteBit(MOTOR, STBY, 0);
}/*}}}*/

void change_LeftMotorSpeed(float speed)/*{{{*/
{
  if(speed > 0)
    forward();
  else {
    backward();
    speed *= -1;
  }
  
  L_PWM->CCR2 = speed; // CCR2 because we are using channel two of TIM3
}/*}}}*/

void change_RightMotorSpeed(float speed)/*{{{*/
{
  if(speed > 0)
    forward();
  else {
    backward();
    speed *= -1;
  }

  R_PWM->CCR2 = speed; // CCR2 because we are using channel two of TIM5
}/*}}}*/

void leftForward(void)
{
  GPIO_SetBits(MOTOR, STBY);
  // CW
  GPIO_SetBits(MOTOR, LEFTIN1);
  GPIO_ResetBits(MOTOR, LEFTIN2);
}
void rightForward(void)
{
  GPIO_SetBits(MOTOR, STBY);
  // CCW
  GPIO_ResetBits(MOTOR, RIGHTIN1);
  GPIO_SetBits(MOTOR, RIGHTIN2);
}
void leftBackward(void)
{
  GPIO_SetBits(MOTOR, STBY);
  // CCW
  GPIO_SetBits(MOTOR, LEFTIN2);
  GPIO_ResetBits(MOTOR, LEFTIN1);
}
void rightBackward(void)
{
  GPIO_SetBits(MOTOR, STBY);
  // CW
  GPIO_ResetBits(MOTOR, RIGHTIN2);
  GPIO_SetBits(MOTOR, RIGHTIN1);
}

/*****************************************************************************/
// Turns
/*****************************************************************************/

void leftTurn(void)
{
  leftForward();
  rightBackward();
  change_LeftMotorSpeed(150);
  change_RightMotorSpeed(150);
  while(L_ENC->CNT < 5000);
  change_LeftMotorSpeed(0);
  change_RightMotorSpeed(0);
}
void rightTurn(void)
{
  leftBackward();
  rightForward();
  change_RightMotorSpeed(120);
  change_LeftMotorSpeed(115);
  while(R_ENC->CNT < 5000);
  change_LeftMotorSpeed(0);
  change_RightMotorSpeed(0);
}


/*****************************************************************************/
// Stop
/*****************************************************************************/

void stopFrontWall(void)
{
  //ADC_Read();
//  change_LeftMotorSpeed(330  - sensor_readings(L_IR));
  //change_RightMotorSpeed(330 - sensor_readings(R_IR));
  //L_ENC->CNT = 5000;
  //R_ENC->CNT = 5000;
  
}

/*
void stopFrontWall(void)
{
  if(sensor_buffers[L_IR] > 330)
  {
    leftSpeed = 0;
    change_LeftMotorSpeed(leftSpeed);
  }
  if(sensor_buffers[R_IR] > 330)
  {
    rightSpeed = 0;
    change_RightMotorSpeed(rightSpeed);
  }

}

*/



