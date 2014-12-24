// Constants
// IR_OUT:
// 1 - left
// 2 - front left
// 3 - front right
// 4 - right

#define IR_OUT1 37
#define IR_OUT2 38
#define IR_OUT3 39
#define IR_OUT4 40
#define IR_IN1 8
#define IR_IN2 9
#define IR_IN3 10
#define IR_IN4 11

#define THRESHOLD 999
#define KP 999
#define KD 999
#define ADJUST 999

#ifndef HIGH
#define HIGH 1
#endif

#ifndef LOW
#define LOW 0
#endif

// Global variables
float pastError=0, currentError=0, 
      leftIR=0, rightIR=0, 
      leftEncoder=0, rightEncoder=0, 
      leftSpeed=0, rightSpeed=0;

void pid();
