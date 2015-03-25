#include "mpu6050.h"

void MPU_Configuration()
{
  IIC_GPIO_Config();
  IIC_Config();
}

uint8_t MPU_ReadID()
{
  uint8_t tmp = 0;

  /* Read device ID  */
  tmp = I2C_ReadDeviceRegister(MPU_ADDR, DEVID);

  /* Return the ID */
  return tmp;
}
