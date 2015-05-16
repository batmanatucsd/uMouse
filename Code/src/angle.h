#ifndef _ANGLE_H_
#define _ANGLE_H_

#include "mcu_lib.h"
#define Filter_gain 0.95
#define rad2degree 57.3

float accel_scale_fact = 8192;
float gyro_scale_fact = 65.5; // Scale factor variables
int time_scale = 5000;
float angle[3] = {0};

void Angle_SetInitial();
void MPU6050_UpdateAngle();

#endif
