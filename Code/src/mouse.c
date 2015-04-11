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

/*****************************************************************************/
// Turns
/*****************************************************************************/
/*
void turnLeft(void)
{
  leftSpeed = x;
  rightSpeed = -x;
  change_LeftMotorSpeed(leftSpeed);
  change_RightMotorSpeed(rightSpeed);
  while(L_ENC->CNT < 5000);
  change_LeftMotorSpeed(0);
  change_RightMotorSpeed(0);
}
void turnRight(void)
{
  leftSpeed = -x;
  rightSpeed = x;
  change_RightMotorSpeed(rightSpeed);
  change_LeftMotorSpeed(leftSpeed);
  while(R_ENC_CNT < 5000);
  change_LeftMotorSpeed(0);
  change_RightMotorSpeed(0);
}

*/
/*****************************************************************************/
// Stop
/*****************************************************************************/
/*
void stopFrontWall(void)
{
  if(sensor_buffers[L_IR] > 330)
  {
    leftSpeed = 0;
  }
  if(sensor_buffers[R_IR] > 330)
  {
    rightSpeed = 0;
  }

  change_RightMotorSpeed(rightSpeed);
  change_LeftMotorSpeed(leftSpeed);
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



