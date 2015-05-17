#ifndef _ANGLE_H_
#define _ANGLE_H_

#include "stm32f10x.h"
#include "MPU6050.h"
#define Filter_gain 0.95
#define rad2degree 57.3
#define dt 20
#define g 9.81

void Angle_Set();
void Angle_Update(uint16_t count_diff);
void Angle_Simple();
void Angle_Handler();
void Angle_OffsetCal();

#endif
