// Constants
// IR_OUT:
// 1 - left
// 2 - front left
// 3 - front right
// 4 - right
#define LF_IR 2
#define RF_IR 1

#define THRESHOLD 200
#define KP 0.00195
#define KD 0.245
#define ADJUST 100

#define LEFT_MAX_SPEED 170
#define RIGHT_MAX_SPEED 190

#ifndef HIGH
#define HIGH 1
#endif

#ifndef LOW
#define LOW 0
#endif

/*****************************************************************************/
// Functions
/*****************************************************************************/
void pid();
