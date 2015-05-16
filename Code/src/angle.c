void MPU6050_SetInitialAngle()
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

  angle = 0;

  MPU6050_GetZGyro(&last_angle_speed);

  last_count = TIM6 -> CNT;
}

void MPU6050_UpdateAngle()
{
  int k = 0.95;
  float current_angle_speed;
  MPU6050_GetZGyro(&current_angle_speed);
  uint16_t current_count = TIM6 ->CNT;

  float error = current_angle_speed - last_angle_speed;
  uint16_t t_diff = current_count - last_count;

  angle += k * error * t_diff / 5000;

  if (angle>360)
  {
    angle -= 360;
  }
  else if (angle<-360)
  {
    angle += 360;
  }

  last_angle_speed = current_angle_speed;
  last_count = TIM6 -> CNT;

  if (last_count> 0xAAAA)
  {
    TIM6 -> CNT = 0;
    last_count = TIM6 -> CNT;
  }
}
