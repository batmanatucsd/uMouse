#ifndef _MOUSE_H_
#define _MOUSE_H_

#define ON 1
#define OFF 0

#define L_IR 3
#define LF_IR 2
#define RF_IR 1
#define R_IR 0

#define LEFT_MAX_SPEED 250
#define RIGHT_MAX_SPEED 250

// TODO: change to appropriate states
typedef enum {
  GO, // should be the first state
  TEST,
  STOP // should be the last state
} state;

typedef enum {
  PAUSE,
  TURN90,
  TURN180,
  FORWARD
} status;

/*****************************************************************************/
// Global Variables
/*****************************************************************************/
volatile state mouse_state;
static status mouse_status = PAUSE;
__IO uint16_t sensor_buffers[4];

/*****************************************************************************/
// General Functions
/*****************************************************************************/
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
void forward(void);
void backward(void);
void turnMotorOff(void);
void change_LeftMotorSpeed(float);
void change_RightMotorSpeed(float);
void leftForward(void);
void rightForward(void);
void leftBackward(void);
void rightBackward(void);

/*****************************************************************************/
// Turns
/*****************************************************************************/
void leftTurn(void);
void rightTurn(void);
void fullTurn(void);
void stopFrontWall(void);

// TODO: Add functions for turns
#endif
