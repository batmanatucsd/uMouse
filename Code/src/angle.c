#include "angle.h"
#include <math.h>

float accel_scale_fact = 8192;
float gyro_scale_fact = 65.5;
int time_scale = 5000;

float angle[3] = {0};
float scaled[6];
int offset[6] = {0};
float tangle[6] = {0};

uint16_t last_count;

void Angle_SetInitial()
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  TIM_TimeBaseStructure.TIM_Prescaler = 14400 -1;  //5kHz
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 100;    //20ms
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM6, ENABLE);

  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
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

void TIM4_IRQHandler()
{
  TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
  Angle_ReadRaw();

  uint16_t dt = 20;

  tangle[3] = (scaled[3]*((float)dt/time_scale)+angle[0]);
  tangle[4] = (scaled[4]*((float)dt/time_scale)+angle[1]);
  tangle[5] = (scaled[5]*((float)dt/time_scale)+angle[2]);

  tangle[2] = atan(scaled[2]/(simu_sqrt(scaled[1]*scaled[1]+scaled[0]*scaled[0])))*(float)rad2degree;
  tangle[1] =-atan(scaled[1]/(simu_sqrt(scaled[1]*scaled[1]+scaled[2]*scaled[2])))*(float)rad2degree;
  tangle[0] = atan(scaled[0]/(simu_sqrt(scaled[0]*scaled[0]+scaled[2]*scaled[2])))*(float)rad2degree;

  angle[0] = Filter_gain*tangle[3]+(1-Filter_gain)*tangle[0];
  angle[1] = Filter_gain*tangle[4]+(1-Filter_gain)*tangle[1];
  angle[2] = Filter_gain*tangle[5]+(1-Filter_gain)*tangle[2];

  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
}
