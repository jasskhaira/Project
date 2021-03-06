#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54114_cm4.h"
#include "fsl_debug_console.h"




/* TODO: insert other include files here. */

#include "fsl_ctimer.h"



/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "fsl_device_registers.h"




/* Usart includes. */
#include "fsl_usart_freertos.h"
#include "fsl_usart.h"



/* TODO: insert other definitions and declarations here. */


#define CTIMER CTIMER0		// Timer 1
#define CTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_AsyncApbClk)

#define LEFT_MOTOR1 kCTIMER_Match_0		//J2[9]
#define LEFT_MOTOR2 kCTIMER_Match_2		//J2[7]


#define RIGHT_MOTOR1 kCTIMER_Match_3		//J2[16]
#define RIGHT_MOTOR2 kCTIMER_Match_1		//J1[10]


#define DEMO_USART USART0
#define DEMO_USART_IRQHandler FLEXCOMM0_IRQHandler
#define DEMO_USART_IRQn FLEXCOMM0_IRQn
#define USART_NVIC_PRIO 5


/*
 * @brief   Application entry point.
 */

uint8_t background_buffer[32];
uint8_t recv_buffer[1];

usart_rtos_handle_t handle;
struct _usart_handle t_handle;

struct rtos_usart_config usart_config = {
    .baudrate = 115200,
    .parity = kUSART_ParityDisabled,
    .stopbits = kUSART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

xQueueHandle queue1= NULL;

void MotorsSetup()
{
	ctimer_config_t config;
	    uint32_t srcClock_Hz;
	    uint32_t timerClock;

	    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
	    SYSCON->ASYNCAPBCTRL = 1;

	    CLOCK_AttachClk(kFRO12M_to_ASYNC_APB);
	    srcClock_Hz = CTIMER_CLK_FREQ;

	CTIMER_GetDefaultConfig(&config);


	    CTIMER_Init(CTIMER, &config);


	    CTIMER_SetupPwm(CTIMER,LEFT_MOTOR1,0,24000U,CTIMER_CLK_FREQ,false);
	    CTIMER_SetupPwm(CTIMER,LEFT_MOTOR2,0,24000U,CTIMER_CLK_FREQ,false);
	    CTIMER_SetupPwm(CTIMER,RIGHT_MOTOR1,0,24000U,CTIMER_CLK_FREQ,false);
	    CTIMER_SetupPwm(CTIMER,RIGHT_MOTOR2,0,24000U,CTIMER_CLK_FREQ,false);
	    CTIMER_StartTimer(CTIMER);
}



void Move(uint8_t speed)
{
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR1, speed);
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR2, 0);


	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR1, 0);
	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR2, speed);

}


void Turn_SlowLeft()
{
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR1, 75);
		CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR2, 0);


		CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR1, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR2, 75);

}
void Turn_SlowRight()
{
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR1, 75);
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR2, 0);


		CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR1, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR2, 75);
}
void Turn_Left()
{
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR1, 100);
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR2, 0);


	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR1, 0);
	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR2, 100);
}
void Turn_Right()
{
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR1, 100);
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR2, 0);


	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR1, 0);
	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR2, 100);

}

/* Api is used to stop the robot */

void Stop(){
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR1, 0);
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR2, 0);


	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR1, 0);
	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR2, 0);
}



void Reverse(){

	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR1, 0);
	CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR2, 100);


	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR1, 100);
	CTIMER_UpdatePwmDutycycle(CTIMER, RIGHT_MOTOR2, 0);

}

static void Uart_task(void *pvParameters)
{
	char send,d;
	 int error;
	 size_t n;
	 usart_config.srcclk = BOARD_DEBUG_UART_CLK_FREQ;
	 usart_config.base = DEMO_USART;

	 NVIC_SetPriority(DEMO_USART_IRQn, USART_NVIC_PRIO);
	if (0 > USART_RTOS_Init(&handle, &t_handle, &usart_config))
			       {
			          // vTaskSuspend(NULL);
			       }


 /*        USART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
         printf("%s\n",recv_buffer[0]);
         printf("%s",recv_buffer[1]);
         if(n>0)
         {
			send=recv_buffer[0];
			xQueueSend(queue1,&send,1000);
			printf("data is send %s",send);
		} */
	       do
	       {
	          // error = USART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
	          /* if (error == kStatus_USART_RxRingBufferOverrun)
	           {
	               /* Notify about hardware buffer overrun
	               if (kStatus_Success !=
	                   USART_RTOS_Send(&handle, (uint8_t *)send_buffer_overrun, strlen(send_buffer_overrun)))
	               {
	                   vTaskSuspend(NULL);
	               }
	           }*/
	           if (n > 0)
	           {
	        	   d=recv_buffer[0];
	        	   printf("%s",d);
	        	   if(d =='M')
	        	   printf("%s",recv_buffer);

	        	 /* send back the received data */

	               USART_RTOS_Send(&handle, (uint8_t *)recv_buffer, n);

	           }
	           vTaskDelay(1000);
	       } while (kStatus_Success == error);

		}




static void Drive_task(void *pvParameters)
{

	char recv;

	while(1){
		xQueueReceive(queue1,&recv,10);
		if(recv=='M')
		{
			Move(80);
			printf("moving");
		}
		else if(recv=='L')
		{
			Turn_Left();
			printf("left");
		}
		else if(recv=='R')
		{
			Turn_Right();
			printf("right");
		}
		else if(recv=='S')
		{
			Stop();
			printf("stop");
		}
		else if(recv=='B')
		{
			 Reverse();
			 printf("reverse");
		}

		printf("data is recived %s",recv);

		vTaskDelay(1000);
	}}

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
    MotorsSetup();
    queue1=xQueueCreate(1,sizeof(char));

    if (xTaskCreate(Uart_task, "Uart_task", configMINIMAL_STACK_SIZE + 10, NULL,2, NULL) != pdPASS)
       {
           PRINTF("Task creation failed!.\r\n");
           while (1)
               ;
       }

    if (xTaskCreate(Drive_task, "Robot_driving_task", configMINIMAL_STACK_SIZE + 10, NULL,  1, NULL) != pdPASS)
          {
              PRINTF("Task creation failed!.\r\n");
              while (1)
                  ;
          }

       vTaskStartScheduler();
       for (;;)
           ;
   }
