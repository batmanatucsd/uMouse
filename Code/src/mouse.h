#ifndef _MOUSE_H
#define _MOUSE_H

#include "mcu_lib.h"

#define ON 1
#define OFF 0

// TODO: change to appropriate states
typedef enum {
  GO,
  STOP
} state;

/*****************************************************************************/
// Global Variables
/*****************************************************************************/
state mouse_state; 

/*****************************************************************************/
// General Functions
/*****************************************************************************/
void delay(volatile int);
void listen_for_button(void);

/*****************************************************************************/
// Motor Controls
/*****************************************************************************/
/************************
   IN1 IN2 STBY Mode
   -----------------
   H   H   H    SHORT BREAK
   L   H   H    CCW
   H   L   H    CW
   L   L   H    STOP
           L    STBY
*************************/
void turnMotorOn(void);
void turnMotorOff(void);
void change_LeftMotorSpeed(uint16_t);
void change_RightMotorSpeed(uint16_t);

/*****************************************************************************/
// Turns
/*****************************************************************************/
// TODO: Add functions for turns
#endif