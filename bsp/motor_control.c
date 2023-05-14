#include <stdint.h>
#include <string.h>
#include "board.h"
#include "pwm.h"
#include "lib.h"
#include "motor_control.h"
#include "lsm303agr.h"
#include "estimator.h"
#include <math.h>
#include "../rtx/cmsis_os2.h"

float accData[3] = {0.0, 0.0, 0.0};
float magData[3] = {0.0, 0.0, 0.0};
float angles[3] = {0.0, 0.0, 0.0};

enum states state = stop;

void init_motor_control(void)
{
    accReadXYZ(accData);
    magReadXYZ(magData);
    referenceHeading = estimate_heading_z(accData, magData, angles);
}

// uint8_t lDutyCycle, rDutyCycle;
// int frequency = 345;
// int lFrequency = 345;
// int rFrequency = 350;

void move(enum states* state)
{
    accReadXYZ(accData);
    magReadXYZ(magData);
    currentHeading = estimate_heading_z(accData, magData, angles);

    error = referenceHeading - currentHeading;
    if (error > 250)
    {
        error = error - 360;
    }
    else if (error < -250)
    {
        error = error + 360;
    }

    controlSignal = kP * error;
    int base;

    switch (*state)
    {
    case forward:
        base = 60;
        rControlSignal = base - 2 + controlSignal;
        lControlSignal = (100 - base) + controlSignal;
        puts1("CF ");
        break;

    case backward:
        base =40;
        rControlSignal = base - 2 + controlSignal;
        lControlSignal = (100 - base) + controlSignal;
        puts1("CB ");
        break;

    case left:
        rControlSignal = 50 + controlSignal;
        lControlSignal = 50 + controlSignal;
        puts1("CL ");
        break;

    case right:
        rControlSignal = 50 + controlSignal;
        lControlSignal = 50 + controlSignal;
        puts1("CR ");
        break;
    
    case stop:
        rControlSignal = STOPPING_PWM;
        lControlSignal = STOPPING_PWM;
        puts1("CS ");
        break;

    default:
        rControlSignal = STOPPING_PWM;
        lControlSignal = STOPPING_PWM;
        break;
    }

    if (rControlSignal < 1)
    {
        rControlSignal = 1;
    }
    else if (rControlSignal > 50)
    {
        rControlSignal = 50;
    }

    if (lControlSignal < 50)
    {
        lControlSignal = 50;
    }
    else if (lControlSignal > 99)
    {
        lControlSignal = 99;
    }

    // printf("lDutyCycle = %d, rDutyCycle = %d\n\r", lControlSignal, rControlSignal);
    pwm_out(PWM_CH1, PWM_FREQUENCY, rControlSignal);
    pwm_out(PWM_CH2, PWM_FREQUENCY, lControlSignal);
    osDelay(100);
}