#include "angle.h"
#include <math.h>

float accel_scale_fact = (float)4*g*0.0305;
float gyro_scale_fact = (float)500*0.0305;
int time_scale = 5000;

int offset[6] = {0};
float scaled[6];
float tangle[6] = {0};
float angle[3] = {0};

uint16_t last_count;

//update angle in milli degree / second
void Angle_Simple()
{
  uint16_t current_count = TIM6 -> CNT;
  uint16_t count_diff = current_count - last_count;

  int16_t raw_data[6];
  MPU6050_GetRawAccelGyro(raw_data);

  if(count_diff > dt * 5)
  {
    scaled[5] = (float)(raw_data[5]-offset[5])*gyro_scale_fact/time_scale;
    angle[2] += scaled[5]*count_diff;
  }


  last_count = current_count;
  if (last_count > 0xAAAA)
  {
    TIM6 -> CNT = (TIM6 -> CNT) - last_count;
    last_count = 0;
  }
}

void Angle_Set()
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

  RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, DISABLE);

  TIM_TimeBaseStructure.TIM_Prescaler = 14400 -1;  //5kHz
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;    //20ms
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  // TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
  //
  // NVIC_InitTypeDef NVIC_InitStructure;
  //
  // NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  // NVIC_Init(&NVIC_InitStructure);
  //
  // TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

  TIM_Cmd(TIM6, ENABLE);
}

// void TIM6_IRQHandler()
// {
//   Angle_Update();
// }

void Angle_Handler()
{
  uint16_t current_count = TIM6 -> CNT;
  if(current_count - last_count > dt * 5)
  {
    Angle_Update();
  }

  last_count = current_count;
  if (last_count > 0xAAAA)
  {
    TIM6 -> CNT = (TIM6 -> CNT) - last_count;
    last_count = TIM6 -> CNT;
  }
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

void Angle_Update()
{
  //TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
  Angle_ReadRaw();

  tangle[3] = (scaled[3]*((float)dt/time_scale)+angle[0]);
  tangle[4] = (scaled[4]*((float)dt/time_scale)+angle[1]);
  tangle[5] = (scaled[5]*((float)dt/time_scale)+angle[2]);

  tangle[2] = atan(scaled[2]/(simu_sqrt(scaled[1]*scaled[1]+scaled[0]*scaled[0])))*(float)rad2degree;
  tangle[1] =-atan(scaled[1]/(simu_sqrt(scaled[1]*scaled[1]+scaled[2]*scaled[2])))*(float)rad2degree;
  tangle[0] = atan(scaled[0]/(simu_sqrt(scaled[0]*scaled[0]+scaled[2]*scaled[2])))*(float)rad2degree;

  angle[0] = Filter_gain*tangle[3]+(1-Filter_gain)*tangle[0];
  angle[1] = Filter_gain*tangle[4]+(1-Filter_gain)*tangle[1];
  angle[2] = Filter_gain*tangle[5]+(1-Filter_gain)*tangle[2];

  //TIM6 -> CNT = 0;
  //TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
}

void MPU6050_OffsetCal()
{
  int i;
  int16_t raw_data[6];
  MPU6050_GetRawAccelGyro(raw_data);

  offset[0]=raw_data[0];
  offset[1]=raw_data[1];
  offset[2]=raw_data[2];
  offset[3]=raw_data[3];
  offset[4]=raw_data[4];
  offset[5]=raw_data[5];

  for (i=1;i<=200;i++){
    MPU6050_GetRawAccelGyro(raw_data);
    offset[0]=(offset[0]+raw_data[0])/2;
    offset[1]=(offset[1]+raw_data[1])/2;
    offset[2]=(offset[2]+raw_data[2])/2;
    offset[3]=(offset[3]+raw_data[3])/2;
    offset[4]=(offset[4]+raw_data[4])/2;
    offset[5]=(offset[5]+raw_data[5])/2;
  }

  offset[2]=offset[2]-(float)g*1000/accel_scale_fact;
}
