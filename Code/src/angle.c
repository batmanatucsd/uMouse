#include "angle.h"
#include <math.h>

uint16_t last_count;
float scaled[6];
int offset[6] = {0};
float tangle[6] = {0};

void Angle_SetInitial()
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  TIM_TimeBaseStructure.TIM_Prescaler = 14400 -1;  //5kHz
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;    //never expect to
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  TIM_Cmd(TIM6, ENABLE);
}

void Angle_ReadRaw()
{
  int16_t raw_data[6];
  MPU6050_GetRawAccelGyro(raw_data);

  scaled[0] = (float)(raw_data[0]-offset[0])*accel_scale_fact/time_scale;
  scaled[1] = (float)(raw_data[1]-offset[1])*accel_scale_fact/time_scale;
  scaled[2] = (float)(raw_data[2]-offset[2])*accel_scale_fact/time_scale;

  scaled[3] = (float)(raw_data[3]-offset[3])*gyro_scale_fact/time_scale;
  scaled[4] = (float)(raw_data[4]-offset[4])*gyro_scale_fact/time_scale;
  scaled[5] = (float)(raw_data[5]-offset[5])*gyro_scale_fact/time_scale;
}

float simu_sqrt(float a)
{
  double x = 1;
  int i = 20;
  while (i--)
  {
    x = (x + a / x) /2;
  }
  return (float) x;
}

void MPU6050_UpdateAngle()
{
  Angle_ReadRaw();

  uint16_t current_count = TIM6 ->CNT;
  uint16_t dt = current_count - last_count;

  tangle[3] = (scaled[3]*((float)dt/time_scale)+angle[0]);
  tangle[4] = (scaled[4]*((float)dt/time_scale)+angle[1]);
  tangle[5] = (scaled[5]*((float)dt/time_scale)+angle[2]);

  tangle[2] = atan(scaled[2]/(simu_sqrt(scaled[1]*scaled[1]+scaled[0]*scaled[0])))*(float)rad2degree;
  tangle[1] =-atan(scaled[1]/(simu_sqrt(scaled[1]*scaled[1]+scaled[2]*scaled[2])))*(float)rad2degree;
  tangle[0] = atan(scaled[0]/(simu_sqrt(scaled[0]*scaled[0]+scaled[2]*scaled[2])))*(float)rad2degree;

  angle[0] = Filter_gain*tangle[3]+(1-Filter_gain)*tangle[0];
  angle[1] = Filter_gain*tangle[4]+(1-Filter_gain)*tangle[1];
  angle[2] = Filter_gain*tangle[5]+(1-Filter_gain)*tangle[2];

  last_count = current_count;

  if (TIM6 -> CNT > 0xAAAA)
  {
    TIM6 -> CNT = (TIM6 -> CNT)- last_count;
    last_count = TIM6 -> CNT;
  }
}
