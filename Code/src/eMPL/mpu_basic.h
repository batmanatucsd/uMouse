#ifndef _MPU_BASIC_H_
#define _MPU_BASIC_H_

//Set up MPU dmp Driver
//init IIC and delay
int MPU_init(void);

//param: cal_data = {pitch, row, yaw}
void MPU6050_Pose(float cal_data[3]);

#endif
