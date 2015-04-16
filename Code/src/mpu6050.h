#ifndef __MPU6050_H
#define __MPU6050_H

#include <stm32f10x.h>
#include "iic.h"

//I2C Slave Address
#define MPU_ADDR  0x53<<1
//which means 0xA6 for a write and 0xA7 for a read.

//I2C Slave Register
#define DEVID 0x00

void MPU_Configuration();
uint8_t MPU_ReadID();

#endif
