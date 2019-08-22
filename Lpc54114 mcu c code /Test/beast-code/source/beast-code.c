#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54114_cm4.h"
#include "fsl_debug_console.h"



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
#define LM0 kCTIMER_Match_0			//	J1[19]					PWM Pin connected to left motor PIN1
#define RM0 kCTIMER_Match_1			//  J2[18]					PWM Pin connected to left motor PIN2
#define LM1 kCTIMER_Match_2			//  J1[16]					PWM Pin connected to right motor PIN1
#define RM1 kCTIMER_Match_0			//  J2[17]					PWM Pin connected to right motor PIN2


#define DEMO_USART USART0
#define DEMO_USART_IRQHandler FLEXCOMM0_IRQHandler
#define DEMO_USART_IRQn FLEXCOMM0_IRQn


/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)
#define USART_NVIC_PRIO 5

static void uart_task(void *pvParameters);				//Task responsible for receiving data from beaglebone
static void Drive_task(void *pvParameters);				//This Task is used to drive motor
static void Ultrasonic_Task(void *pvParameters);		//This Task associate with Ultarsonic Sensor to avoid obstracle
static void Object_Search();							//This Task used to search teh object



void MotorsSetup();										// It set up PWM to drive motors
void Move();											// It is used to move in forward direction
void Turn_SlowLeft();									// It is used to turn left with slow speed
void Turn_SlowRight();									// It is used to turn right with slow speed
void Turn_Left();										// It is used to turn left
void Turn_Right();										// It is used to turn right
void Stop();											// It is used to Stop robot
void Reverse();											// It is used to reverse the robot
float Front_Obstarcle();								// It gives the distance of front obstacle
float Rear_Obstarcle();									// It gives the distance of rear obstacle
void Search();											// It drive the motors to search object
void Circle();											// It drive the motors to make circle


uint8_t background_buffer[100];							// For receiving data from Beaglebone in background
uint8_t recv_buffer[1];

usart_rtos_handle_t handle;								// USART handle
struct _usart_handle t_handle;


/*This structure contain the configurations of USART
 * USART runs at 9600 baudrate 8N1
 */

struct rtos_usart_config usart_config = {
    .baudrate    = 9600,
    .parity      = kUSART_ParityDisabled,
    .stopbits    = kUSART_OneStopBit,
    .buffer      = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

/* Queue Handle  */


xQueueHandle Obj_track= NULL;
xQueueHandle queue1= NULL;

/* Task Handles   */

TaskHandle_t Uart_Task_Handle=NULL;
TaskHandle_t Ultrasonic_Task_Handle=NULL;
TaskHandle_t Drive_task_Handle=NULL;
TaskHandle_t Object_Search_Handle=NULL;


int main(void)

{



	CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
	SYSCON->ASYNCAPBCTRL = 1;



    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();


    MotorsSetup();

    /* Creating Queues for InterTask communication */

    queue1=xQueueCreate(1,sizeof(uint8_t));
    Obj_track=xQueueCreate(1,sizeof(uint8_t));


    /* Creating Tasks for rtos */

    if (xTaskCreate(uart_task, "Uart_task", configMINIMAL_STACK_SIZE + 10, NULL, 3
    		,&Uart_Task_Handle) != pdPASS)
         {
             PRINTF("Task creation failed!.\r\n");
             while (1)
                 ;
         }


    if (xTaskCreate(Drive_task, "Robot_driving_task", configMINIMAL_STACK_SIZE + 10, NULL,2,&Drive_task_Handle) != pdPASS)
                {
                    PRINTF("Task creation failed!.\r\n");
                    while (1)
                        ;
                }

    if (xTaskCreate(Ultrasonic_Task, "Ultrasonic_Task", configMINIMAL_STACK_SIZE + 10, NULL,4,&Ultrasonic_Task_Handle) != pdPASS)
    		                {
    		                    PRINTF("Task creation failed!.\r\n");
    		                    while (1)
    		                        ;
    		                }
    if (xTaskCreate(Object_Search, "Object_Search", configMINIMAL_STACK_SIZE + 10, NULL,0,&Object_Search_Handle) != pdPASS)
          		   {
          			   PRINTF("Task creation failed!.\r\n");
          			   while (1);


          	      }



         vTaskStartScheduler();
         for (;;)
             ;
     }





static void uart_task(void *pvParameters)
 {

	vTaskSuspend(Object_Search_Handle);
	int error,status=0;
	uint8_t send;
     size_t n            = 0;
     usart_config.srcclk = BOARD_DEBUG_UART_CLK_FREQ;
     usart_config.base   = DEMO_USART;

     NVIC_SetPriority(DEMO_USART_IRQn, USART_NVIC_PRIO);



     USART_RTOS_Init(&handle, &t_handle, &usart_config);


     while(1)
     {
    	 /* Receive the data form USART */

         error=USART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
         //printf("%c",recv_buffer);
         if (error == kStatus_USART_RxRingBufferOverrun)
                {
        	 printf("Buffer overrun");

                }


         if (n > 0)
         {
        	 send=recv_buffer[0];

        	 if(send=='F')
       		{
       			//send1=recv_buffer[0];
       			if(status==0)
       			{
       			vTaskSuspend(Drive_task_Handle);
       			vTaskResume(Object_Search_Handle);
       			//printf("drive suspend\n");
       			//printf("search resume\n");
       			status=1;
       			}
       			xQueueSend(Obj_track,&send,10);


       		}
       		 else if(send=='S')
       		  {
       			 Stop();
       			vTaskSuspend(Object_Search_Handle);

       		  }

       		  else
       		   {
       			  if(status==1)
       			  {
       				vTaskSuspend(Object_Search_Handle);
       				vTaskResume(Drive_task_Handle);
       				//printf("drive resume\n");
       				//printf("search suspend\n");
       				  status=0;

       			  }

       			xQueueSend(queue1,&send,10);
       		   }



       		//printf("%c",recv_buffer);
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
	  			Move(90);
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
	  			vTaskDelay(5);
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
	  			xQueueSend(Obj_track,&recv,10);
	  			recv='n';
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
    	 if(Front_obs<10)
    	 {	Stop();
    	 	 vTaskSuspend(Uart_Task_Handle);
    		 vTaskSuspend(Drive_task_Handle);
    		 vTaskSuspend(Object_Search_Handle);

    		 vTaskDelay(10);
    		 Reverse();
    		 vTaskDelay(150);
    		 /*Turn_Left();
    		 vTaskDelay(200);
    		 Turn_Right();
    		 vTaskDelay(200);*/
    		 Stop();
    		 vTaskResume(Uart_Task_Handle);
    		 vTaskResume(Drive_task_Handle);
    		 vTaskResume(Object_Search_Handle);
    		}

    	 if(Rear_obs<10)
    	 {	Stop();
    		 vTaskSuspend(Uart_Task_Handle);
    		 vTaskSuspend(Object_Search_Handle);
    		 vTaskResume(Drive_task_Handle);
    		 vTaskDelay(5);
    		 Move(90);
    		 vTaskDelay(150);
    		// Turn_Left();
    		 //vTaskDelay(200);
    		 //Move();
    		 //vTaskDelay(80);
    		 //Turn_Right();
    		 //vTaskDelay(100);
    		 Stop();
    		 vTaskResume(Uart_Task_Handle);
    		 vTaskResume(Object_Search_Handle);
    		 vTaskResume(Drive_task_Handle);
    		}






 	}

 }


 static void Object_Search(void *pvParameters)
 {
	 uint8_t Obj_recv;
 	while(1)
 	{
 		printf("finding\n");
 	xQueueReceive(Obj_track,&Obj_recv,10);
 	if(Obj_recv=='F')
 	{
 	Circle();
 	Move(75);
 	vTaskDelay(300);
 	Search();
 	Stop();


 	}
 	Obj_recv='n';
 }}






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


 void Move(int speed)
 {
		CTIMER_UpdatePwmDutycycle(CTIMER, LM0, speed);
		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, RM0, speed);
		CTIMER_UpdatePwmDutycycle(CTIMER1, RM1, 0);


 }


 void Turn_SlowLeft()
 {
	 	CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 0);
 		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
 		CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 70);
 		CTIMER_UpdatePwmDutycycle(CTIMER1, RM1, 0);

 }


 void Turn_SlowRight()
 {
		CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 70);
		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 0);
 		CTIMER_UpdatePwmDutycycle(CTIMER1, RM1,0);
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
		CTIMER_UpdatePwmDutycycle(CTIMER1, RM1,0);

 }



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
		CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 80);
		CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 0);
		CTIMER_UpdatePwmDutycycle(CTIMER1,RM1, 80);

 }



 void Search()
 {
 	 CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 75);
 	 CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
 	 CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 0);
 	 CTIMER_UpdatePwmDutycycle(CTIMER1,RM1, 75);
 	 vTaskDelay(750);

 }

 void Circle()
 {
 	CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 90);
 	CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
 	CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 70);
 	CTIMER_UpdatePwmDutycycle(CTIMER1,RM1, 0);
 	vTaskDelay(1000);

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
