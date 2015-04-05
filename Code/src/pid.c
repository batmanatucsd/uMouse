#include "pid.h"
#include "mouse.h"

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
    if(difference > 20 || difference < -22)
      currentError = difference;
      GPIO_SetBits(GPIOC, GREEN); // GREEN
      GPIO_ResetBits(GPIOB, RED);
      GPIO_ResetBits(GPIOC, YELLOW);
	}
  else if (sensor_buffers[LF_IR] < THRESHOLD && sensor_buffers[RF_IR] >= THRESHOLD)
  {  // ONLY RIGHT WALL
		currentError =  NOLEFTWALL_THRESHOLD - sensor_buffers[RF_IR];
      GPIO_ResetBits(GPIOC, GREEN);
      GPIO_SetBits(GPIOB, RED); // RED
      GPIO_ResetBits(GPIOC, YELLOW);
	}
  else if (sensor_buffers[LF_IR] >= THRESHOLD && sensor_buffers[RF_IR] < THRESHOLD)
  { // ONLY LEFT WALL
		currentError = sensor_buffers[LF_IR] - NORIGHTWALL_THRESHOLD;
      GPIO_ResetBits(GPIOC, GREEN);
      GPIO_ResetBits(GPIOB, RED);
      GPIO_SetBits(GPIOC, YELLOW); // YELLOW
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
  rightSpeed -= (total); // ADJUST;

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
    leftSpeed = 110;

  /*printf("%d         %f          %f\r\n", total, leftSpeed, rightSpeed);*/
  /*printf("sensor reading: %u         %u        %u        %u\r\n",*/
          /*sensor_buffers[0], sensor_buffers[1], sensor_buffers[2], sensor_buffers[3]);*/
  change_RightMotorSpeed(rightSpeed);
  change_LeftMotorSpeed(leftSpeed);

	pastError = currentError;
  /*Delay_us(15);*/
}
