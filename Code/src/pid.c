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
// pid(void)
//
// @brief: Calculate pid
/*****************************************************************************/
void pid(void)/*{{{*/
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
    if(last_case == BOTH)
      currentError = sensor_buffers[LF_IR] - sensor_buffers[RF_IR] - OFFSET;

    // Set indicating LED's
    GPIO_SetBits(GPIOC, GREEN); // GREEN
    GPIO_ResetBits(GPIOB, RED);
    GPIO_ResetBits(GPIOC, YELLOW);
    last_case = BOTH;

  } else if (sensor_buffers[LF_IR] < LEFT_THRESHOLD && sensor_buffers[RF_IR] >= RIGHT_THRESHOLD) { 
    // *********** ONLY RIGHT WALL *********** //
    currentError =  RIGHTWALL_TARGET - sensor_buffers[RF_IR];

    // Set indicating LED's
    GPIO_ResetBits(GPIOC, GREEN);
    GPIO_SetBits(GPIOB, RED); // RED
    GPIO_ResetBits(GPIOC, YELLOW);

    last_case = RIGHT;

  } else if (sensor_buffers[LF_IR] >= LEFT_THRESHOLD && sensor_buffers[RF_IR] < RIGHT_THRESHOLD) {
    // *********** ONLY LEFT WALL *********** //
    currentError = sensor_buffers[LF_IR] - LEFTWALL_TARGET;

    // Set indicating LED's
    GPIO_ResetBits(GPIOC, GREEN);
    GPIO_ResetBits(GPIOB, RED);
    GPIO_SetBits(GPIOC, YELLOW); // YELLOW

    last_case = LEFT;

  } else if (sensor_buffers[LF_IR] < LEFT_THRESHOLD && sensor_buffers[RF_IR] < RIGHT_THRESHOLD) {
    // *********** NO WALL *********** //
    // TODO: finish the one with no walls
    currentError = (rightEncoder - leftEncoder) * ADJUST ; // * some constant

    float newSpeed = (leftSpeed > rightSpeed) ? leftSpeed : rightSpeed;
    /*change_RightMotorSpeed(newSpeed);*/
    /*change_LeftMotorSpeed(newSpeed);*/

    // Set indicating LED's
    GPIO_SetBits(GPIOC, GREEN);
    GPIO_SetBits(GPIOB, RED);
    GPIO_SetBits(GPIOC, YELLOW); // YELLOW
    last_case = NOWALLS;
  }

  printf("%u       %u\r\n", sensor_buffers[LF_IR], sensor_buffers[RF_IR]);
  /*printf("      %u      %u      %u\r\n", OFFSET, LEFTWALL_TARGET, RIGHTWALL_TARGET);*/

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

/*****************************************************************************/
// calibrate(void)
//
// @brief: Calibrates the sensors
/*****************************************************************************/
void calibrate(void) /*{{{*/
{
  ADC_Read(1, 1, 1, 1); 
  OFFSET = sensor_buffers[LF_IR] - sensor_buffers[RF_IR];
  RIGHTWALL_TARGET = sensor_buffers[RF_IR];
  LEFTWALL_TARGET = sensor_buffers[LF_IR];
}/*}}}*/
