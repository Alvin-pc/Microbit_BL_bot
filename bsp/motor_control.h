#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
#include <stdint.h>
#include <stdio.h>

#define kPbase 0.1
#define STOPPING_PWM 50

extern float referenceHeading;
extern float currentHeading;
extern float controlSignal;
extern int error;
extern int lControlSignal;
extern int rControlSignal;
extern float kP;

// float referenceHeading=0.0;
// float currentHeading=0.0;
// float controlSignal=0.0;
// int error=0;
// int lControlSignal=0;
// int rControlSignal=0;
// float kP = kPbase;

enum states {forward, backward, left, right, stop} ;


void init_motor_control(void);
void move(enum states* state);

#endif  /* MOTOR_CONTROL_H */