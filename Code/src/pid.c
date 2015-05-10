#include "pid.h"

/*****************************************************************************/
// PID Globals
/*****************************************************************************/
static int32_t pastError = 0, currentError = 0;
static float leftEncoder=0, rightEncoder=0, 
             leftSpeed=0, rightSpeed=0;

__IO uint16_t sensor_buffers[4];
__IO uint16_t sensor_readings[4];
float difference = 0 ;

/*****************************************************************************/
// pid
//
// PID code
/*****************************************************************************/
void pid()
{
  ADC_Read();
	
  /////////////////////
  // BOTH WALLS
  /////////////////////
  if ((sensor_buffers[LF_IR] >= LEFT_THRESHOLD &&  sensor_buffers[RF_IR] >= RIGHT_THRESHOLD))
  {
    difference = map_RF() - map_LF();
    if(difference > 20 || difference < -22)
      currentError = difference;

      GPIO_SetBits(GPIOC, GREEN); // GREEN
      GPIO_ResetBits(GPIOB, RED);
      GPIO_ResetBits(GPIOC, YELLOW);
  }
  /////////////////////
  // ONLY RIGHT WALL
  /////////////////////
  else if (sensor_buffers[LF_IR] < LEFT_THRESHOLD && sensor_buffers[RF_IR] >= RIGHT_THRESHOLD)
  { 
    currentError =  TARGET - map_RF();

      GPIO_ResetBits(GPIOC, GREEN);
      GPIO_SetBits(GPIOB, RED); // RED
      GPIO_ResetBits(GPIOC, YELLOW);
  }
  /////////////////////
  // ONLY LEFT WALL
  /////////////////////
  else if (sensor_buffers[LF_IR] >= LEFT_THRESHOLD && sensor_buffers[RF_IR] < RIGHT_THRESHOLD)
  {
    currentError = map_LF() - TARGET;

      GPIO_ResetBits(GPIOC, GREEN);
      GPIO_ResetBits(GPIOB, RED);
      GPIO_SetBits(GPIOC, YELLOW); // YELLOW
  }
  /////////////////////
  // NO WALLS
  ///////////////////// 
  // TODO: finish the one with no walls
  /*else if (sensor_buffers[LF_IR] < LEFT_THRESHOLD && sensor_buffers[RF_IR] < RIGHT_THRESHOLD)*/
  /*{*/
		/*currentError = (rightEncoder - leftEncoder) * ADJUST ; // * some constant*/
	/*}*/

  /*printf("%u       %u\r\n", currentError, pastError);*/

  // calculate the total adjustment
  float total = KP * currentError + KD * (currentError - pastError);

  // Change the motor speed
  leftSpeed += total;
  rightSpeed -= (total);
  
  // Limit the minimum speed
  // note: mabye we don't need this. try one without this checking
  if(rightSpeed < 0)
    rightSpeed = 120;

  if(leftSpeed < 0)
    leftSpeed = 120;

  /*printf("%d         %f          %f\r\n", total, leftSpeed, rightSpeed);*/
  /*printf("sensor reading: %u         %u        %u        %u\r\n",*/
          /*sensor_buffers[0], sensor_buffers[1], sensor_buffers[2], sensor_buffers[3]);*/

  /*printf("%f             %f\r\n", map_LF(), map_RF());*/
  change_RightMotorSpeed(rightSpeed);
  change_LeftMotorSpeed(leftSpeed);
	pastError = currentError;
  /*Delay_us(15);*/
}

/*****************************************************************************/
// map_readings
//
// Map sensor readings
/*****************************************************************************/
float map_RF() {
  if(sensor_buffers[RF_IR] > IN_MAX) 
    sensor_buffers[RF_IR] = IN_MAX;
  return distance_RF[sensor_buffers[RF_IR]*RATIO];
}

float map_LF() {
  if(sensor_buffers[LF_IR] > IN_MAX) 
    sensor_buffers[LF_IR] = IN_MAX;
  return distance_LF[sensor_buffers[LF_IR]*RATIO];
}


