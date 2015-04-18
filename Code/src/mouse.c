#include "mouse.h"

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
void turnMotorOn(void)/*{{{*/
{
  GPIO_SetBits(MOTOR, STBY);

  // CW
  GPIO_SetBits(MOTOR, LEFTIN1);
  GPIO_ResetBits(MOTOR, LEFTIN2);

  // CCW
  GPIO_ResetBits(MOTOR, RIGHTIN1);
  GPIO_SetBits(MOTOR, RIGHTIN2);
}/*}}}*/

void turnMotorOff(void)/*{{{*/
{
  // TODO: should we change the way we turn off the motors?
  // perhaps turn the inputs off or the pwm (might save battery time)
  GPIO_WriteBit(MOTOR, STBY, 0);
}/*}}}*/

void change_LeftMotorSpeed(float speed)/*{{{*/
{
  TIM3->CCR2 = speed;
}/*}}}*/

void change_RightMotorSpeed(float speed)/*{{{*/
{
  TIM5->CCR2 = speed;
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

void rightTurn(void)
{
  
  L_ENC->CNT = 0; 
  R_ENC->CNT = 0; 
  leftForward();
  rightBackward();
  change_LeftMotorSpeed(250);
  change_RightMotorSpeed(250);
  while(L_ENC->CNT < 2125);
  change_LeftMotorSpeed(0);
  change_RightMotorSpeed(0);
  L_ENC->CNT = 0; 
  R_ENC->CNT = 0; 
}
void leftTurn(void)
{
  L_ENC->CNT = 0; 
  R_ENC->CNT = 0; 
  leftBackward();
  rightForward();
  change_RightMotorSpeed(250);
  change_LeftMotorSpeed(250);
  while(R_ENC->CNT < 2100);
  change_LeftMotorSpeed(0);
  change_RightMotorSpeed(0);
  L_ENC->CNT = 0; 
  R_ENC->CNT = 0; 
}


/*****************************************************************************/
// Stop
/*****************************************************************************/
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
/*
void stopFrontWall(void)
{
  if(sensor_buffers[L_IR] > 70)
  {
    while(leftSpeed > 0)
    {
      leftSpeed = LEFT_MAX_SPEED - 10; 
      change_LeftMotorSpeed(leftSpeed);
    }
  }
  if(sensor_buffers[R_IR] > 70)
  {
    while(rightSpeed > 0) 
    {
      rightSpeed = RIGHT_MAX_SPEED - 11;
      change_LeftMotorSpeed(leftSpeed);
    }
  }
}
*/



