#include "pid.h"

/*****************************************************************************/
// PID Globals
/*****************************************************************************/
static float pastError = 0, currentError = 0, sumError = 0;
static float leftEncoder=0, rightEncoder=0, 
             leftSpeed=0, rightSpeed=0;
static PID_CASE last_case = BOTH;

__IO uint16_t sensor_buffers[4];
__IO uint16_t sensor_readings[4];
float difference = 0 ;

/*****************************************************************************/
// pid
//
// PID code
/*****************************************************************************/
void pid()/*{{{*/
{
  ADC_Read(0, 1, 1, 0);
  float total;

  // reset encoder counts after one cell
  /*if(L_ENC->CNT > 8500 || R_ENC->CNT > 8500) {*/
    /*L_ENC->CNT = 0;*/
    /*R_ENC->CNT = 0;*/
  /*}*/

  // Calculate errors depending on different cases
  if (sensor_buffers[LF_IR] >= LEFT_THRESHOLD && sensor_buffers[RF_IR] >= RIGHT_THRESHOLD) {
    // *********** BOTH WALLS *********** //
    last_case = BOTH;
    if(last_case == BOTH)
      currentError = sensor_buffers[LF_IR] - sensor_buffers[RF_IR];

    // Set indicating LED's
    GPIO_SetBits(GPIOC, GREEN); // GREEN
    GPIO_ResetBits(GPIOB, RED);
    GPIO_ResetBits(GPIOC, YELLOW);

  } else if (sensor_buffers[LF_IR] < LEFT_THRESHOLD && sensor_buffers[RF_IR] >= RIGHT_THRESHOLD) { 
    // *********** ONLY RIGHT WALL *********** //
    last_case = RIGHT;
    currentError =  RIGHTWALL_TARGET - sensor_buffers[RF_IR];

    // Set indicating LED's
    GPIO_ResetBits(GPIOC, GREEN);
    GPIO_SetBits(GPIOB, RED); // RED
    GPIO_ResetBits(GPIOC, YELLOW);

  } else if (sensor_buffers[LF_IR] >= LEFT_THRESHOLD && sensor_buffers[RF_IR] < RIGHT_THRESHOLD) {
    // *********** ONLY LEFT WALL *********** //
    last_case = LEFT;
    currentError = sensor_buffers[LF_IR] - LEFTWALL_TARGET;

    // Set indicating LED's
    GPIO_ResetBits(GPIOC, GREEN);
    GPIO_ResetBits(GPIOB, RED);
    GPIO_SetBits(GPIOC, YELLOW); // YELLOW

  } else if (sensor_buffers[LF_IR] < LEFT_THRESHOLD && sensor_buffers[RF_IR] < RIGHT_THRESHOLD) {
    // *********** NO WALL *********** //
    // TODO: finish the one with no walls
    last_case = NOWALLS;
    currentError = (rightEncoder - leftEncoder) * ADJUST ; // * some constant

    float newSpeed = (leftSpeed > rightSpeed) ? leftSpeed : rightSpeed;
    /*change_RightMotorSpeed(newSpeed);*/
    /*change_LeftMotorSpeed(newSpeed);*/

    // Set indicating LED's
    GPIO_SetBits(GPIOC, GREEN);
    GPIO_SetBits(GPIOB, RED);
    GPIO_SetBits(GPIOC, YELLOW); // YELLOW
  }

  /*printf("%u       %u\r\n", sensor_buffers[LF_IR], sensor_buffers[RF_IR]);*/

  // calculate the total adjustment
  total = KP * currentError + KD * (currentError - pastError) + KI * sumError;

  // Change the motor speed
  leftSpeed += total;
  rightSpeed -= (total);
  
  // Limit the maximum speed
  if(rightSpeed > RIGHT_MAX_SPEED)
    rightSpeed = RIGHT_MAX_SPEED;

  if(leftSpeed > LEFT_MAX_SPEED)
    leftSpeed = LEFT_MAX_SPEED;

  // Limit the minimum speed
  if(rightSpeed < 0)
    rightSpeed = 170;

  if(leftSpeed < 0)
    leftSpeed = 170;


  // Change motor speeds
  change_RightMotorSpeed(rightSpeed);
  change_LeftMotorSpeed(leftSpeed);

  // Update errors
	pastError = currentError;
  sumError += currentError;
  Delay_us(50);
}/*}}}*/

