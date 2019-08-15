#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54114_cm4.h"
#include "fsl_debug_console.h"


/* TODO: insert other include files here. */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "fsl_usart_freertos.h"
#include "fsl_usart.h"

#include "fsl_ctimer.h"

/* TODO: insert other definitions and declarations here. */

#define CTIMER CTIMER0                 /* Timer 0 */
#define LM0 kCTIMER_Match_0			//	J1[19]
#define RM0 kCTIMER_Match_1			//  J2[18]
#define LM1 kCTIMER_Match_2			//  J1[16]
#define RM1 kCTIMER_Match_0			//  J2[17]


#define DEMO_USART USART0
#define DEMO_USART_IRQHandler FLEXCOMM0_IRQHandler
#define DEMO_USART_IRQn FLEXCOMM0_IRQn


/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)
#define USART_NVIC_PRIO 5

static void uart_task(void *pvParameters);
static void Drive_task(void *pvParameters);
static void Ultrasonic_Task(void *pvParameters);



void MotorsSetup();
void Move();
void Turn_SlowLeft();
void Turn_SlowRight();
void Turn_Left();
void Turn_Right();
void Stop();
void Reverse();
float Front_Obstarcle();
float Rear_Obstarcle();
void Search();

uint8_t background_buffer[100];
uint8_t recv_buffer[1];

usart_rtos_handle_t handle;
struct _usart_handle t_handle;


struct rtos_usart_config usart_config = {
    .baudrate    = 9600,
    .parity      = kUSART_ParityDisabled,
    .stopbits    = kUSART_OneStopBit,
    .buffer      = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

xQueueHandle queue1= NULL;
TaskHandle_t Uart_Task_Handle=NULL;
TaskHandle_t Ultrasonic_Task_Handle=NULL;
TaskHandle_t Drive_task_Handle=NULL;


int main(void)

{

	CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
	SYSCON->ASYNCAPBCTRL = 1;


    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();


    MotorsSetup();

    queue1=xQueueCreate(1,sizeof(uint8_t));


    if (xTaskCreate(uart_task, "Uart_task", configMINIMAL_STACK_SIZE + 10, NULL, 2,&Uart_Task_Handle) != pdPASS)
         {
             PRINTF("Task creation failed!.\r\n");
             while (1)
                 ;
         }


    if (xTaskCreate(Drive_task, "Robot_driving_task", configMINIMAL_STACK_SIZE + 10, NULL,2,&Ultrasonic_Task_Handle) != pdPASS)
                {
                    PRINTF("Task creation failed!.\r\n");
                    while (1)
                        ;
                }

    if (xTaskCreate(Ultrasonic_Task, "Ultrasonic_Task", configMINIMAL_STACK_SIZE + 10, NULL,2,&Drive_task_Handle) != pdPASS)
    		                {
    		                    PRINTF("Task creation failed!.\r\n");
    		                    while (1)
    		                        ;
    		                }


         vTaskStartScheduler();
         for (;;)
             ;
     }





static void uart_task(void *pvParameters)
 {

	int error;
	uint8_t send;
     size_t n            = 0;
     usart_config.srcclk = BOARD_DEBUG_UART_CLK_FREQ;
     usart_config.base   = DEMO_USART;

     NVIC_SetPriority(DEMO_USART_IRQn, USART_NVIC_PRIO);

     USART_RTOS_Init(&handle, &t_handle, &usart_config);

     /* Receive user input and send it back to terminal. */
     while(1)
     {
         error=USART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
         //printf("%c",recv_buffer);
         if (error == kStatus_USART_RxRingBufferOverrun)
                {
        	 printf("Buffer overrun");

                }

         if (n > 0)
         {
       		send=recv_buffer[0];
       		//printf("%c",recv_buffer);
       		xQueueSend(queue1,&send,10);
         }
     }

 }



 static void Drive_task(void *pvParameters)
 {
	 uint8_t recv;

	  	while(1){
	  		xQueueReceive(queue1,&recv,10);
	  		if(recv=='M')
	  		{
	  			Move();
	  			//printf("moving\n");
	  			recv='n';
	  		}
	  		else if(recv=='L')
	  		{
	  			Turn_Left();
	  			//printf("left\n");
	  			recv='n';
	  		}
	  		else if(recv=='R')
	  		{
	  			Turn_Right();
	  			//printf("right\n");
	  			recv='n';
	  		}
	  		else if(recv=='S')
	  		{
	  			Stop();
	  			//printf("stop");
	  			recv='n';

	  		}
	  		else if(recv=='B')
	  		{
	  			Stop();
	  			vTaskDelay(10);
	  			 Reverse();
	  			// printf("reverse\n");
	  			recv='n';
	  		}

	  		else if(recv=='l')
	  		{
	  			Turn_SlowLeft();
	  			// printf("reverse\n");
	  			 recv='n';
	  			  		}
	  		else if(recv=='r')
	  		{
	  			Turn_SlowRight();
	 			// printf("reverse\n");
	  			 recv='n';
	  		}
	  		else if(recv=='F')
	  		{
	  		Search();
	  		vTaskDelay(100);
	  		Move();
	  		vTaskDelay(800);


	  	}
 }
 }



 static void Ultrasonic_Task(void *pvParameters)
 {
 	float Front_obs,Rear_obs;


     while(1)


 	{
       	 Front_obs=Front_Obstarcle();
    	 Rear_obs=Rear_Obstarcle();
    	 if(Front_obs<8)
    	 {	Stop();
    		 vTaskSuspend(Uart_Task_Handle);
    		 //vTaskSuspend(Drive_task_Handle);

    		 Reverse();
    		 vTaskDelay(100);
    		 Turn_Left();
    		 vTaskDelay(100);
    		 Turn_Right();
    		 vTaskDelay(100);
    		 Stop();
    		 vTaskResume(Uart_Task_Handle);
    		// vTaskResume(Drive_task_Handle);
    		}

    	 if(Rear_obs<8)
    	 {	Stop();
    		 vTaskSuspend(Uart_Task_Handle);
    		 //vTaskSuspend(Drive_task_Handle);

    		 Move();
    		 vTaskDelay(100);
    		 Turn_Left();
    		 vTaskDelay(100);
    		 Turn_Right();
    		 vTaskDelay(100);
    		 Stop();
    		 vTaskResume(Uart_Task_Handle);
    		// vTaskResume(Drive_task_Handle);
    		}






 	}

 }



 void MotorsSetup()
 {
			ctimer_config_t config;
			uint32_t srcClock_Hz;
			srcClock_Hz = CLOCK_GetFreq(kCLOCK_BusClk);



			CTIMER_GetDefaultConfig(&config);


			CTIMER_Init(CTIMER, &config);
			CTIMER_Init(CTIMER1, &config);
			CTIMER_Init(CTIMER2, &config);
			CTIMER_Init(CTIMER3, &config);

			CTIMER_SetupPwm(CTIMER,LM0,0,20000,srcClock_Hz,NULL);
			CTIMER_SetupPwm(CTIMER,LM1,0,20000,srcClock_Hz,NULL);
			CTIMER_SetupPwm(CTIMER,RM0,0,20000,srcClock_Hz,NULL);
			CTIMER_SetupPwm(CTIMER1,RM1,0,20000,srcClock_Hz,NULL);
			CTIMER_StartTimer(CTIMER);
			CTIMER_StartTimer(CTIMER1);
 }


 void Move()
 {
		CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 90);
		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 90);
		CTIMER_UpdatePwmDutycycle(CTIMER1, RM1, 0);


 }


 void Turn_SlowLeft()
 {
	 	CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 40);
 		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
 		CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 0);
 		CTIMER_UpdatePwmDutycycle(CTIMER1, RM1, 40);

 }


 void Turn_SlowRight()
 {
		CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 40);
		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 0);
 		CTIMER_UpdatePwmDutycycle(CTIMER1, RM1, 40);
 }


 void Turn_Left()
 {
		CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, RM0,90);
		CTIMER_UpdatePwmDutycycle(CTIMER1, RM1,0);
 }



 void Turn_Right()

 {
		CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 90);
		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER1, RM1, 0);

 }

 /* Api is used to stop the robot */

 void Stop()
 {
		CTIMER_UpdatePwmDutycycle(CTIMER, LM0,0);
		CTIMER_UpdatePwmDutycycle(CTIMER, LM1,0);
		CTIMER_UpdatePwmDutycycle(CTIMER, RM0,0);
		CTIMER_UpdatePwmDutycycle(CTIMER1,RM1,0);
 }



 void Reverse()
 {
		CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 90);
		CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER1,RM1, 90);

 }

 void Search()
 {
	 CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 0);
	 CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 60);
	 CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 60);
	 CTIMER_UpdatePwmDutycycle(CTIMER1,RM1, 0);

 }
 float Front_Obstarcle()
 {

	 	float Front_time,Front_distance;
		GPIO_PinWrite(BOARD_Front_trig_GPIO,BOARD_Front_trig_PORT,BOARD_Front_trig_PIN,1);
		vTaskDelay(10);
		GPIO_PinWrite(BOARD_Front_trig_GPIO,BOARD_Front_trig_PORT,BOARD_Front_trig_PIN,0);

		while(GPIO_PinRead(BOARD_Front_echo_GPIO,BOARD_Front_echo_PORT,BOARD_Front_echo_PIN)==0);

		CTIMER_StartTimer(CTIMER2);

		while(GPIO_PinRead(BOARD_Front_echo_GPIO,BOARD_Front_echo_PORT,BOARD_Front_echo_PIN)==1);
		CTIMER_StopTimer(CTIMER2);

		Front_time= CTIMER_GetTimerCountValue(CTIMER2);

		Front_distance =(0.0343*(Front_time/96))/2;

		CTIMER_Reset(CTIMER2);

		return Front_distance;

 }


 float Rear_Obstarcle()
 {		float Rear_time,Rear_distance;

		GPIO_PinWrite(BOARD_Rear_trig_GPIO,BOARD_Rear_trig_PORT,BOARD_Rear_trig_PIN,1);
		vTaskDelay(10);
		GPIO_PinWrite(BOARD_Rear_trig_GPIO,BOARD_Rear_trig_PORT,BOARD_Rear_trig_PIN,0);

		while(GPIO_PinRead(BOARD_Rear_echo_GPIO,BOARD_Rear_echo_PORT,BOARD_Rear_echo_PIN)==0);

		CTIMER_StartTimer(CTIMER3);

		while(GPIO_PinRead(BOARD_Rear_echo_GPIO,BOARD_Rear_echo_PORT,BOARD_Rear_echo_PIN)==1);
		CTIMER_StopTimer(CTIMER3);

		Rear_time= CTIMER_GetTimerCountValue(CTIMER3);
		Rear_time=Rear_time/96;
		Rear_distance =(0.0343*Rear_time)/2;

		CTIMER_Reset(CTIMER3);


		return Rear_distance;
 }
