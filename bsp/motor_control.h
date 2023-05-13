#ifndef CONTROL_H
#define CONTROL_H
#include <stdint.h>

#define kPbase 0.1
#define STOPPING_PWM 50

float referenceHeading =0.0;
float currentHeading = 0.0;
float controlSignal = 0.0;
int error =0;
int lControlSignal=0;
int rControlSignal=0;
float kP = kPbase;

enum states {forward, backward, left, right, stop} ;
enum states state = stop;

void init_motor_control(void);
void move(enum states* state);

#endif  /* CONTROL_H */