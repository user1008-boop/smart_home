#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stm32f10x.h>
#include "motor.h"

#define STEPPER_ANGLE		0.703125

enum dir{Pos, Neg};   //Pos = 0, Neg = 1

#define IN1	PCout(0)


#define IN2	PCout(1)


#define IN3	PCout(2)


#define IN4	PCout(3)


void ULN2003_Configuration(void);
void stepper(unsigned int dir, unsigned int speed);
void angle_control(unsigned int dir, double angle, unsigned int speed);

#endif
