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

/*****************************************************************************/
// Turns
/*****************************************************************************/
