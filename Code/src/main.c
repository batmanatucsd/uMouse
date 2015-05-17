#include "mouse.h"
#include "angle.h"
#include "anbt_dmp_driver.h"
#include "anbt_dmp_fun.h"
#include "anbt_dmp_mpu6050.h"

/*****************************************************************************/
// Global Variables
/*****************************************************************************/
__IO uint16_t sensor_buffers[4];
__IO uint16_t sensor_readings[4];
extern float angle[3];
extern int16_t raw_data[6];
extern int16_t offset[6];

/*****************************************************************************/
// Main Function
/*****************************************************************************/
int main(void)
{
  // MCU Configurations
  RCC_Configuration();
  GPIO_Configuration();
  //DMA_Configuration();
  NVIC_Configuration();



  //ADC_Configuration();
  PWM_Configuration();
  ENCODER_Configuration();
<<<<<<< HEAD
//  MPU6050_I2C_Init();
//  MPU6050_Initialize();
//  Angle_Set();
=======
>>>>>>> d8c63bb26d9bd3d686e8f8bee02724a034dcd19a

//  MPU6050_I2C_Init();

// Only for debug
USART_Configuration();
ANBT_I2C_Configuration();
  AnBT_DMP_MPU6050_Init();
  //MPU6050_Initialize();
//  Angle_Set();
//  Angle_OffsetCal();




  mouse_state = STOP;
  mouse_status = FORWARD;

  /*turnMotorOn();*/
  while(1)
  {

    listen_for_button();

    switch(mouse_state) {
      case GO:

        switch(mouse_status) {
          case FORWARD:
           /*ADC_Read(1, 0, 0, 1);*/
            /*if(sensor_buffers[L_IR] > 120 && sensor_buffers[R_IR] > 120)*/
              /*stopFrontWall();*/
            /*else {  // Do PID when moving forward*/
          //    change_LeftMotorSpeed(175);
            //  change_RightMotorSpeed(175);
            //  pid();
            /*}*/
/*
            Delay_us(100);
            break;

          case TURN90:
            Delay_us(1000000);
            Delay_us(1000000);
            rightTurn();
            break;

          case TURN180:
            fullTurn();
            Delay_us(100);
            mouse_status = FORWARD;
            break;
        
*/

/*****************************************CELL BY CELL MOVEMENT****************************/

	

	
	
	L_ENC->CNT=0;
	R_ENC->CNT=0;

	while(L_ENC->CNT < 6005 && R_ENC->CNT < 6005){
		change_LeftMotorSpeed(80);
		change_RightMotorSpeed(80);
		pid();
		
		if(L_ENC->CNT >3000 && L_ENC->CNT<4000){
			change_LeftMotorSpeed(60);
			change_RightMotorSpeed(60);
			pid();
		}

		if(L_ENC->CNT>=4000 && L_ENC->CNT<5000){
			change_LeftMotorSpeed(40);
			change_RightMotorSpeed(40);
			pid();
		}
	
	}
		change_LeftMotorSpeed(0);
		change_RightMotorSpeed(0);

	if(sensor_buffers[L_IR]>150){
		//calibration
		while(){
			
		}
	}
	
	Delay_us(1000000);
	L_ENC->CNT=0;
	R_ENC->CNT=0;


/******************************LEFT 90 TURN******************************************/
/*	ADC_Read(1,0,0,1);	

	if(sensor_buffers[L_IR] > 120 && sensor_buffers[R_IR] > 120){
		stopFrontWall();
		
		if(sensor_buffers[L_IR]>400){
			
			change_LeftMotorSpeed(0);
			change_RightMotorSpeed(0);	
	
			Delay_us(1000);
			leftTurn();
			Delay_us(10000);			

		}
	}
	else{
		change_LeftMotorSpeed(150);
		change_RightMotorSpeed(150);
		pid();
	
	}
*/


/*******************************TEST SMOOTH RIGHT***********************************/
/*	ADC_Read(1,1,1,1);

	change_LeftMotorSpeed(150);
	change_RightMotorSpeed(150);
	pid();
	rightTurn();
*/	

      break;
  }

      case TEST:


/******************************straight, stop, right, etc *****************/
/*
	ADC_Read(1,0,0,1);	

	if(sensor_buffers[L_IR] > 120 && sensor_buffers[R_IR] > 120){
		stopFrontWall();
		
		if(sensor_buffers[L_IR]>400){
			
			change_LeftMotorSpeed(0);
			change_RightMotorSpeed(0);	
	
			Delay_us(1000);
			rightTurn();
			Delay_us(10000);			

		}
	}
	else{
		change_LeftMotorSpeed(150);
		change_RightMotorSpeed(150);
		pid();
	}

*/
/*******************************straight, stop, 180, go ********************/
/*
	ADC_Read(1,0,0,1);
	if(sensor_buffers[L_IR] > 120 && sensor_buffers[R_IR] > 120){
		stopFrontWall();

		if(sensor_buffers[L_IR]>590){
    	
			Delay_us(1000);
			fullTurn();
			Delay_us(10000);
		}
	}
	else{
		change_LeftMotorSpeed(150);
		change_RightMotorSpeed(150);
		pid();
	}

*/

       
		

        GPIO_SetBits(GPIOC, YELLOW);
        GPIO_SetBits(GPIOC, GREEN);
        GPIO_ResetBits(GPIOB, RED);
        break;

      case STOP:

        ADC_Read(1, 1, 1, 1);
        turnMotorOff();
        GPIO_SetBits(GPIOB, RED);
        GPIO_ResetBits(GPIOC, GREEN);
        GPIO_SetBits(GPIOC, YELLOW);

        /*printf("  sensor reading: %u         %u        %u        %u\r\n",*/
              /*sensor_buffers[3], sensor_buffers[2], sensor_buffers[1], sensor_buffers[0]);*/

        /*Free our mouse*/
        if(sensor_buffers[R_IR] >= 450) {
          Delay_us(10000);
          mouse_state = GO;
        }
        /*if(sensor_buffers[L_IR] >= 450) {*/
          /*Delay_us(10000);*/
          /*calibrate();*/
          /*GPIO_SetBits(GPIOC, GREEN);*/
          /*Delay_us(10000);*/
        /*}*/

        break;
    }


>>>>>>> origin/mpu-test
    /*printf("%u            %u\r\n", TIM3->CCR3, TIM4->CCR4);*/
    /*printf("                                                  %u              %u\r\n", L_ENC->CNT, R_ENC->CNT);*/
ADC_Read(1,1,1,1);
    printf("                                                    %u      %u      %u\r\n",/* OFFSET, LEFTWALL_TARGET, RIGHTWALL_TARGET);*/
            sensor_readings[0], sensor_readings[1], sensor_readings[2], sensor_readings[3]);
            /*ADC1->JDR1, ADC1->JDR2, ADC1->JDR3, ADC1->JDR4);*/
            /*ADC1->JOFR1, ADC1->JOFR2, ADC1->JOFR3, ADC1->JOFR4);*/
    /*printf("%u            %u\r\n", TIM8->CNT, TIM4->CNT);*/
    /*if(TIM8->CNT > 7000)*/
        /*turnMotorOff();*/

  }
}
