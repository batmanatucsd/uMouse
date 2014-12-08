#include "pid.h"

void pid()
{
	// TODO
	// Initialize leftIR and rightIR

	if ((leftIR >= THRESHOLD && rightIR <= THRESHOLD) ||
		(leftIR <= THRESHOLD && rightIR >= THRESHOLD))
	{
		currentError = leftIR - rightIR;
	} else if (leftIR < THRESHOLD)
	{
		currentError =  THRESHOLD - rightIR;
	} else if (rightIR < THRESHOLD)
	{
		currentError = leftIR - THRESHOLD;
	} else if (leftIR < THRESHOLD && rightIR < THRESHOLD)
	{
		currentError = (rightEncoder - leftEncoder); // * some constant
	}

	float proportional = currentError;
	float derivative = currentError - pastError;
	float total = KP * currentError + KD * derivative;

	leftSpeed += total / ADJUST;
	rightSpeed -= total / ADJUST;

	pastError = currentError;
}