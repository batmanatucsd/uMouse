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
    state next_state;

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
    leftForward();
  else {
    leftBackward();
    speed *= -1;
  }
  
  if(speed > LEFT_MAX_SPEED)
    speed = LEFT_MAX_SPEED;
  
  L_PWM->CCR2 = speed; // CCR2 because we are using channel two of TIM3
}/*}}}*/

void change_RightMotorSpeed(float speed)/*{{{*/
{
  if(speed > 0)
    rightForward();
  else {
    rightBackward();
    speed *= -1;
  }

  if(speed > RIGHT_MAX_SPEED)
    speed = RIGHT_MAX_SPEED;

  R_PWM->CCR2 = speed; // CCR2 because we are using channel two of TIM5
}/*}}}*/

void leftForward(void)/*{{{*/
{
  GPIO_SetBits(MOTOR, STBY);
  // CW
  GPIO_SetBits(MOTOR, LEFTIN1);
  GPIO_ResetBits(MOTOR, LEFTIN2);
}/*}}}*/

void rightForward(void)/*{{{*/
{
  GPIO_SetBits(MOTOR, STBY);
  // CCW
  GPIO_ResetBits(MOTOR, RIGHTIN1);
  GPIO_SetBits(MOTOR, RIGHTIN2);
}/*}}}*/

void leftBackward(void)/*{{{*/
{
  GPIO_SetBits(MOTOR, STBY);
  // CCW
  GPIO_SetBits(MOTOR, LEFTIN2);
  GPIO_ResetBits(MOTOR, LEFTIN1);
}/*}}}*/

void rightBackward(void)/*{{{*/
{
  GPIO_SetBits(MOTOR, STBY);
  // CW
  GPIO_ResetBits(MOTOR, RIGHTIN2);
  GPIO_SetBits(MOTOR, RIGHTIN1);
}/*}}}*/


/*****************************************************************************/
// Turns
/*****************************************************************************/
void rightTurn(void)/*{{{*/
{
  /**********************************90 TURN*********************************/
  // reset encoder counts
//  L_ENC->CNT = 0; 
//  R_ENC->CNT = 0; 

//  change_LeftMotorSpeed(250);
//  change_RightMotorSpeed(-250);

//  while(L_ENC->CNT < 2450); // wait for encoder counts

  // stop motors
//  change_LeftMotorSpeed(0);
//  change_RightMotorSpeed(0);

  // reset encoder counts
//  L_ENC->CNT = 0; 
//  R_ENC->CNT = 0;

/********************************SMOOTH RIGHT TURN*************************/
/*  ADC_Read(1,1,1,1);
  while(sensor_buffers[R_IR]<140){
	
	//turn left wheel more AND faster
	while(L_ENC->CNT<5000){
		if(sensor_buffers[R_IR]>140)
			break;
		change_LeftMotorSpeed(255);
	}
  }

*/
 
}/*}}}*/

void leftTurn(void)/*{{{*/
{
  // reset encoder counts
  L_ENC->CNT = 0; 
  R_ENC->CNT = 0; 

  // set motor directions and speed
  /*leftBackward();*/
  /*rightForward();*/
  change_RightMotorSpeed(250);
  change_LeftMotorSpeed(-250);

  while(R_ENC->CNT < 2450); // wait for encoder counts

  // stop motors
  change_LeftMotorSpeed(0);
  change_RightMotorSpeed(0);

  // reset encoder counts
  L_ENC->CNT = 0; 
  R_ENC->CNT = 0; 
}/*}}}*/

void fullTurn(void)/*{{{*/
{
  // reset encoder counts
  L_ENC->CNT = 0; 
  R_ENC->CNT = 0; 

  // set motor directions and speed
  change_RightMotorSpeed(250);
  change_LeftMotorSpeed(-250);

  /*while(R_ENC->CNT < 4250); // wait for encoder counts*/
  while(R_ENC->CNT < 5250); // wait for encoder counts

  // stop motors
  change_LeftMotorSpeed(0);
  change_RightMotorSpeed(0);

  // reset encoder counts
  L_ENC->CNT = 0; 
  R_ENC->CNT = 0; 

}/*}}}*/

/*****************************************************************************/
// Stop
/*****************************************************************************/
void stopFrontWall(void){
  ADC_Read(1, 0, 0, 1);

  if(sensor_buffers[L_IR] >= 120 && sensor_buffers[L_IR] < 350) { 
    // Stop the bot when it is too close
    change_LeftMotorSpeed(430 - (0.1661*sensor_buffers[L_IR] + 358.57));
    change_RightMotorSpeed(400 - (0.1854*sensor_buffers[R_IR] + 326.2));
  } 
  else{
    // Gradually slow down 
    change_LeftMotorSpeed(500-sensor_buffers[L_IR]);
    change_RightMotorSpeed(400-sensor_buffers[R_IR]);

  } 
}



