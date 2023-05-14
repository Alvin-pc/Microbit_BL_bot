#include "motor_control.h"

// extern float referenceHeading;
// extern float currentHeading;
// extern float controlSignal;
// extern int error;
// extern int lControlSignal;
// extern int rControlSignal;
// extern float kP;
// extern enum states state;

int init_controller(void);
void check_controllerMsg(enum states* state);
void add_controllerMsg(char* command);
void execute_driver(char *command, enum states* state);
void task_log(void *args);
void task_stop_log(void);
void task_stop(void);


