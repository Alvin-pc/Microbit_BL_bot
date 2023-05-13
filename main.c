#include <stdint.h>
#include <string.h>
#include "cmsis_os2.h"
#include "ble_uart.h"
#include "board.h"
#include "lib.h"
#include "buggy_controller.h"
#include "utility.h"
#include "bsp/lsm303agr.h"
#include "motor_control.h"


/* OS objects */
osThreadId_t tid_ctrl;
osThreadId_t tid_disp;
osThreadId_t tid_log;


/* Buffer to hold the command received from UART or BLE
 * We use single buffer assuming command-response protocol,
 * that the next command will be sent after receiving the
 * response for the current command.
 */
uint8_t cmd_buf[256];
uint32_t cmd_len;
extern enum states state;

uint8_t pic[5][5] ={        {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0},
                            {0, 0, 0, 0, 0} 
                    };	


extern char msg[];
extern float referenceHeading;
extern float currentHeading;
extern float controlSignal;
extern int error;
extern int lControlSignal;
extern int rControlSignal;
extern float kP;


/* Called from BLE softdevice using SWI2_EGU2_IRQHandler */
static void ble_recv_handler(const uint8_t s[], uint32_t len)
{
    /* Make a local copy so that BLE can receive next characters */
    memcpy(cmd_buf, s, len);

    /* Remove trailing new line or carriange return characters. */
    while ((s[len - 1] == '\n') || (s[len - 1] == '\r'))
        len--;
    cmd_buf[len] = '\0';            // null-terminate the string
    cmd_len = len;

    /* Echo on UART */
    puts1("cmd from BLE : ");
    puts1((char *)cmd_buf);

    add_controllerMsg((char *) cmd_buf);

    /* Signal the waiting task. */
    osThreadFlagsSet(tid_ctrl, 1); 
}

void task_ctrl(void *arg)
{
    while (1)
    {
        /* Receive a command from BLE */
        move(&state);
        
    }
}

// Display Task

void task_disp(void *arg)
{
    while (1)
    {      
        osThreadFlagsWait(1, osFlagsWaitAny, osWaitForever);

        led_display(pic); 
        check_controllerMsg(&state);
       
    }
}
// Display Task

void task_log(void *args)
{
  osThreadFlagsWait(2, osFlagsWaitAny, osWaitForever);
  int log_counter = 0;
  ble_send((uint8_t *)msg, strlen((char *)msg));
  while (1)
  {
    if(log_counter % 100 == 0)
    {
      printMetrics(referenceHeading, currentHeading, error, controlSignal,lControlSignal,rControlSignal, msg);
      puts1("msg = ");
      puts1(msg);
      puts1("\n\r");
      ble_send((uint8_t *)msg, strlen((char *)msg));
    } 
    
    osDelay(1000);    
  }
}

int main(void)
{
    /* BSP initializations before BLE because we are using printf from BSP */
    board_init();
    ble_init(ble_recv_handler);

    pwm_init(PWM_CH1, PIN_3);
    pwm_init(PWM_CH2, PIN_4);

    IMUinit(0 , 0 , 2 , 0 , 0 , 0);
    
    audio_sweep(500, 2000, 100);

    /* Initialize and start the kernel */
    osKernelInitialize();
    init_controller();

    /* controller task */
    tid_ctrl = osThreadNew(task_ctrl, NULL, NULL);
    osThreadSetPriority(tid_ctrl, osPriorityLow);

    /* Display Task */
    tid_disp = osThreadNew(task_disp, NULL, NULL);
    osThreadSetPriority(tid_disp, osPriorityLow2);

    tid_log = osThreadNew(task_log, NULL, NULL);
    osThreadSetPriority(task_log, osPriorityLow1);

    osKernelStart();
    /* never returns */

    led_blink(2, 2, BLINK_FOREVER);

    return 0;
}
