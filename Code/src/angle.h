#ifndef _ANGLE_H_
#define _ANGLE_H_

#include "stm32f10x.h"
#include "MPU6050.h"
#define Filter_gain 0.95
#define rad2degree 57.3
#define dt 20;

void Angle_Set();
void Angle_Update();

#endif
