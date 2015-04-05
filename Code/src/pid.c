#include "pid.h"

/*****************************************************************************/
// PID Globals
/*****************************************************************************/
static int32_t pastError = 0, currentError = 0;
static float leftEncoder=0, rightEncoder=0, 
      leftSpeed=0, rightSpeed=0;

__IO uint16_t sensor_buffers[4];
__IO uint16_t sensor_readings[4];
int32_t difference = 0 ;

/*****************************************************************************/
// pid
//
// PID code
/*****************************************************************************/
void pid()
{
  ADC_Read();

	if ((sensor_buffers[LF_IR] >= THRESHOLD &&  sensor_buffers[RF_IR] >= THRESHOLD))
  { // BOTH WALLS
    difference = sensor_buffers[LF_IR] - sensor_buffers[RF_IR];
    if(difference > 20 || difference < -20)
      currentError = difference;
	}
  else if (sensor_buffers[LF_IR] < THRESHOLD && sensor_buffers[RF_IR] >= THRESHOLD)
  {  // ONLY RIGHT WALL
		currentError =  THRESHOLD - sensor_buffers[RF_IR];
	}
  else if (sensor_buffers[LF_IR] >= THRESHOLD && sensor_buffers[RF_IR] < THRESHOLD)
  { // ONLY LEFT WALL
		currentError = sensor_buffers[LF_IR] - THRESHOLD;
	}
  /*else if (sensor_buffers[LF_IR] < THRESHOLD && sensor_buffers[RF_IR] < THRESHOLD)*/
  /*{*/
		/*currentError = (rightEncoder - leftEncoder) * ADJUST ; // * some constant*/
	/*}*/

  /*printf("%u       %u\r\n", currentError, pastError);*/
  // TODO:
  // set limits to total error value
	float total = KP * currentError + KD * (currentError - pastError);


  leftSpeed += total; // ADJUST;
  rightSpeed -= (total-25); // ADJUST;

  /*if(total >= 0)*/
  /*{*/
    /*leftSpeed += total / ADJUST;*/
    /*rightSpeed -= total / LEFT_MAX_SPEED;*/
  /*}*/
  /*else*/
  /*{*/
    /*leftSpeed += total / RIGHT_MAX_SPEED;*/
    /*rightSpeed -= (total-25)/ ADJUST;*/
  /*}*/
  
  if(rightSpeed > RIGHT_MAX_SPEED)
    rightSpeed = RIGHT_MAX_SPEED;
  else if(rightSpeed < 0)
    rightSpeed = 120;

  if(leftSpeed > LEFT_MAX_SPEED)
    leftSpeed = LEFT_MAX_SPEED;
  else if(leftSpeed < 0)
    leftSpeed = 100;

  printf("%d         %f          %f\r\n", total, leftSpeed, rightSpeed);
  change_RightMotorSpeed(rightSpeed);
  change_LeftMotorSpeed(leftSpeed);

	pastError = currentError;
}
