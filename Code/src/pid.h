// Constants
// IR_OUT:
// 1 - left
// 2 - front left
// 3 - front right
// 4 - right

#define THRESHOLD 200
#define NOLEFTWALL_THRESHOLD 300
#define NORIGHTWALL_THRESHOLD 300
#define KP 0.00120
#define KD 0.250
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
