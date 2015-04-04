#include "pid.h"

/*****************************************************************************/
// pid
//
// PID code
/*****************************************************************************/
void pid()
{
	// TODO:
	// Initialize leftIR and rightIR
  /*
    float leftAmbientLight = 0,
          rightAmbientLight = 0;
    leftAmbientLight = analogRead(IR_IN1);
    rhgtAmbientLight = analogRead(IR_IN4);
    wait(100); // may have to wait like few milliseconds
    digitalWrite(IR_OUT1, HIGH);
    digitalWrite(IR_OUT4, HIGH);
    digitalWrite(IR_OUT1, LOW);
    digitalWrite(IR_OUT4, LOW);
    wait(100); // may have to wait like few milliseconds
    leftIR = analogRead(IR_IN1);
    rightIR = analogRead(IR_IN4);
    leftIR -= leftAmbientLight;
    rightIR -= rightAmbientLight; 
  */

	if ((leftIR >= THRESHOLD && rightIR <= THRESHOLD) ||
		(leftIR <= THRESHOLD && rightIR >= THRESHOLD))
  {
		currentError = leftIR - rightIR;
	} 
  else if (leftIR < THRESHOLD)
  {
		currentError =  THRESHOLD - rightIR;
	} 
  else if (rightIR < THRESHOLD)
  {
		currentError = leftIR - THRESHOLD;
	}
  else if (leftIR < THRESHOLD && rightIR < THRESHOLD)
  {
		currentError = (rightEncoder - leftEncoder) * ADJUST ; // * some constant
	}

  // TODO:
  // set limits to total error value
	float total = KP * currentError + KD * (currentError - pastError);

	leftSpeed += total / ADJUST;
	rightSpeed -= total / ADJUST;

  /*
    setPWM(leftMotor, leftSpeed);
    setPWM(rightMotor, rightSpeed);
  */

	pastError = currentError;
}
