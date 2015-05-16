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
  if(L_ENC->CNT > 8500 || R_ENC->CNT > 8500) {
    L_ENC->CNT = 0;
    R_ENC->CNT = 0;
  }

  /////////////////////
  // BOTH WALLS
  /////////////////////
  if (sensor_buffers[LF_IR] >= LEFT_THRESHOLD && sensor_buffers[RF_IR] >= RIGHT_THRESHOLD)
  {
    last_case = BOTH;
    /*ADC_Read(0, 1, 1, 0);*/
    /*Delay_us(5);*/
    if(last_case == BOTH)
      currentError = sensor_buffers[LF_IR] - sensor_buffers[RF_IR];
    /*if(difference > 22 || difference < -22)*/
      /*currentError = difference;*/

    /*total = KP * currentError + KD * (currentError - pastError) + KI * sumError;*/
    // FOR LINEARIZATION
    /*float difference = map_RF() - map_LF() + 0.15;*/
    /*if(difference < 0.78 || difference > -0.78)*/
      /*currentError = difference;*/

    /*if(currentError < -0.5)*/
      /*currentError += currentError/2;*/

      GPIO_SetBits(GPIOC, GREEN); // GREEN
      GPIO_ResetBits(GPIOB, RED);
      GPIO_ResetBits(GPIOC, YELLOW);
  }
  /////////////////////
  // ONLY RIGHT WALL
  /////////////////////
  else if (sensor_buffers[LF_IR] < LEFT_THRESHOLD && sensor_buffers[RF_IR] >= RIGHT_THRESHOLD)
  /*else if (sensor_buffers[RF_IR] >= RIGHT_THRESHOLD)*/
  { 
    last_case = RIGHT;
    /*ADC_Read(0, 1, 1, 0);*/
    /*Delay_us(5);*/
    currentError =  RIGHTWALL_TARGET - sensor_buffers[RF_IR];
    /*currentError = map_RF() - R_TARGET;*/

    /*total = NOL_KP * currentError + NOL_KD * (currentError - pastError) + KI * sumError;*/
      GPIO_ResetBits(GPIOC, GREEN);
      GPIO_SetBits(GPIOB, RED); // RED
      GPIO_ResetBits(GPIOC, YELLOW);
  }
  /////////////////////
  // ONLY LEFT WALL
  /////////////////////
  else if (sensor_buffers[LF_IR] >= LEFT_THRESHOLD && sensor_buffers[RF_IR] < RIGHT_THRESHOLD)
  /*else if (sensor_buffers[LF_IR] >= LEFT_THRESHOLD)*/
  {
    last_case = LEFT;
    /*ADC_Read(0, 1, 1, 0);*/
    /*Delay_us(5);*/
    currentError = sensor_buffers[LF_IR] - LEFTWALL_TARGET;
    /*printf("%u\r\n", sensor_buffers[LF_IR]);*/
    /*total = NOR_KP * currentError + NOR_KD * (currentError - pastError) + KI * sumError;*/
    /*currentError = L_TARGET - map_LF();*/

      GPIO_ResetBits(GPIOC, GREEN);
      GPIO_ResetBits(GPIOB, RED);
      GPIO_SetBits(GPIOC, YELLOW); // YELLOW
  }
  /////////////////////
  // NO WALLS
  ///////////////////// 
  // TODO: finish the one with no walls
  else if (sensor_buffers[LF_IR] < LEFT_THRESHOLD && sensor_buffers[RF_IR] < RIGHT_THRESHOLD)
  {
    last_case = NOWALLS;
    currentError = (rightEncoder - leftEncoder) * ADJUST ; // * some constant
    /*total = KP * currentError + KD * (currentError - pastError) + KI * sumError;*/
    /*currentError = (leftEncoder - rightEncoder) * ADJUST ; // depend on encoder counts */
    float newSpeed = (leftSpeed > rightSpeed) ? leftSpeed : rightSpeed;
    /*change_RightMotorSpeed(newSpeed);*/
    /*change_LeftMotorSpeed(newSpeed);*/
    // No wall...
      GPIO_SetBits(GPIOC, GREEN);
      GPIO_SetBits(GPIOB, RED);
      GPIO_SetBits(GPIOC, YELLOW); // YELLOW
  }

  /*printf("%u       %u\r\n", sensor_buffers[LF_IR], sensor_buffers[RF_IR]);*/


  // calculate the total adjustment
  /*printf("%f       %f\r\n", map_LF(), map_RF());*/

  total = KP * currentError + KD * (currentError - pastError) + KI * sumError;

  // Change the motor speed
  leftSpeed += total;
  rightSpeed -= (total);
  
  /*printf("%f       %f\r\n", currentError, total);*/
  /*printf("%f       %f\r\n", leftSpeed, rightSpeed);*/

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


  /*printf("%d         %f          %f\r\n", total, leftSpeed, rightSpeed);*/
  /*printf("sensor reading: %u         %u        %u        %u\r\n",*/
          /*sensor_buffers[0], sensor_buffers[1], sensor_buffers[2], sensor_buffers[3]);*/

  /*printf("%f             %f\r\n", map_LF(), map_RF());*/
  change_RightMotorSpeed(rightSpeed);
  change_LeftMotorSpeed(leftSpeed);
	pastError = currentError;
  sumError += currentError;
  Delay_us(25);
}/*}}}*/

void pid_turn(uint16_t target) /*{{{*/
{
  uint16_t err, last_err, errsum;
  float speed;

  /*while(// somecondition) {*/
   
    /*err = target - R_ENC_CNT; // might need to change the encoder*/
    /*drr = err - last_err; */
    
    /*speed = kp*err + kd*drr + ki*errsum;*/
    /*errsum += err;*/

    /*change_RightMotorSpeed(speed);*/
    /*change_LeftMotorSpeed(-speed);*/

    /*// check for the condition*/

  /*}*/
}/*}}}*/

/*****************************************************************************/
// map_readings
//
// Map sensor readings
/*****************************************************************************/
float map_RF() {
  if(sensor_buffers[RF_IR] > IN_MAX)
    return distance_RF[IN_MAX*RATIO];
  else 
    return distance_RF[sensor_buffers[RF_IR]*RATIO];
}

float map_LF() {
  if(sensor_buffers[LF_IR] > IN_MAX) 
    return distance_LF[IN_MAX*RATIO];
  else
    return distance_LF[sensor_buffers[LF_IR]*RATIO];
}

