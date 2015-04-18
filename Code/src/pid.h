// Constants
// IR_OUT:
// 1 - left
// 2 - front left
// 3 - front right
// 4 - right

#define THRESHOLD 200
#define NOLEFTWALL_THRESHOLD 290
#define NORIGHTWALL_THRESHOLD 290
#define KP 0.0005
#define KD 0.250
#define ADJUST 100

#define LEFT_MAX_SPEED 200
#define RIGHT_MAX_SPEED 200

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
