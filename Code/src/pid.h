#ifndef PID_H
#define PID_H

// Constants
// IR_OUT:
// 1 - left
// 2 - front left
// 3 - front right
// 4 - right

#include "mouse.h"
#define RIGHT_THRESHOLD 200
#define LEFT_THRESHOLD 130

#define THRESHOLD 200
#define RIGHTWALL_TARGET 450
#define LEFTWALL_TARGET 450

#define KP 0.00225
#define KD 0.2
#define KI 0
#define ADJUST 600

#ifndef HIGH
#define HIGH 1
#endif

#ifndef LOW
#define LOW 0
#endif

typedef enum {
  BOTH, RIGHT, LEFT, NOWALLS
} PID_CASE;

/*****************************************************************************/
// Functions
/*****************************************************************************/
void pid();

#endif // PID_H

