#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../rtx/cmsis_os2.h"
#include "../bsp/serial.h"
#include "board.h"
#include "buggy_controller.h"
#include "utility.h"
#include "../ble/ble_uart.h"


#define MSGQUEUE_LENGTH 10
//char* msg ="[refHeading,currHeading,error,ctrlSignal,lctrlSignal,rctrlSignal]";
char msg[100];
extern osThreadId_t tid_log;


// osThreadId_t tid_forward;
// osThreadId_t tid_backward;
// osThreadId_t tid_left;
// osThreadId_t tid_right;


// osThreadId_t active_thread[10];
// int active_count = -1;


osMessageQueueId_t controller_MsgQueue;

uint8_t fwd[5][5] = {{0, 0, 1, 0, 0},
                     {0, 1, 1, 1, 0},
                     {0, 0, 1, 0, 0},
                     {0, 0, 1, 0, 0},
                     {0, 0, 1, 0, 0}};

uint8_t rev[5][5] = {{0, 0, 1, 0, 0},
                     {0, 0, 1, 0, 0},
                     {0, 0, 1, 0, 0},
                     {0, 1, 1, 1, 0},
                     {0, 0, 1, 0, 0}};

uint8_t right_rotate[5][5] = {{1, 1, 1, 1, 0},
                       {1, 0, 0, 1, 0},
                       {1, 0, 0, 1, 0},
                       {1, 0, 1, 1, 1},
                       {1, 0, 0, 1, 0}};

uint8_t left_rotate[5][5] = {{0, 1, 1, 1, 1},
                      {0, 1, 0, 0, 1},
                      {0, 1, 0, 0, 1},
                      {1, 1, 1, 0, 1},
                      {0, 1, 0, 0, 1}};

typedef struct
{
  char *command;
  int *values[];
} ControllerMsg;

int init_controller(void)
{
  init_motor_control();
  puts1("init controller\n\r");
  controller_MsgQueue = osMessageQueueNew(MSGQUEUE_LENGTH, sizeof(ControllerMsg), NULL);
  if (controller_MsgQueue == NULL)
  {
    puts1("Message Queue object not created, handle failure\n\r");
  }

  return 0;
}

void check_controllerMsg(enum states* state)
{
  ControllerMsg msg;
  osStatus_t status;
  uint16_t msgCount = osMessageQueueGetCount(controller_MsgQueue);

  // puts1("in check_controllerMsg\n\r");
  for (int i = 0; i < msgCount; i++)
  {
    status = osMessageQueueGet(controller_MsgQueue, &msg, NULL, 0U);
    if (status == osOK)
    {
      char *command = msg.command;
      puts1("command in Cmd Message Queue: ");
      puts1(command);
      puts1("\n\r");
      execute_driver(command, state);
    }
  }
}

void add_controllerMsg(char *command)
{
  ControllerMsg msg;
  msg.command = command;
  osMessageQueuePut(controller_MsgQueue, &msg, 0U, 0U);
  puts1("added the message in the queue with command : ");
  puts1(command);
  puts1("\n\r");
}

void execute_driver(char *command, enum states* state)
{

  if (strcasecmp(command, "FORWARD") == 0)
  {
    puts1("MOVING FORWARD\n\r");
    *state = forward;
  }
  else if (strcasecmp(command, "BACKWARD") == 0)
  {
    puts1("MOVING BACKWARD\n\r");
    *state = backward;
  }
  else if (strcasecmp(command, "LEFT") == 0)
  {
    puts1("MOVING LEFT\n\r");
    *state = left;

  }
  else if (strcasecmp(command, "RIGHT") == 0)
  {
    puts1("MOVING RIGHT\n\r");
    *state = right;

  }
  else if (strcasecmp(command, "STOP") == 0)
  {
    puts1("STOPPING\n\r");
    *state = stop;
  }
  else if (strcasecmp(command, "LOG") == 0)
  {
    puts1("LOGGING\n\r");
    osThreadFlagsSet(tid_log, 2);
  }
}
