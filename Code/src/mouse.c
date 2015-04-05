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

void change_LeftMotorSpeed(uint16_t speed)/*{{{*/
{
  if(speed > LEFT_MAX_SPEED)
    TIM3->CCR2 = LEFT_MAX_SPEED;
  else 
    TIM3->CCR2 = speed;
}/*}}}*/

void change_RightMotorSpeed(uint16_t speed)/*{{{*/
{
  if(speed > RIGHT_MAX_SPEED)
    TIM5->CCR2 = RIGHT_MAX_SPEED;
  else 
    TIM5->CCR2 = speed;
}/*}}}*/

/*****************************************************************************/
// Turns
/*****************************************************************************/
