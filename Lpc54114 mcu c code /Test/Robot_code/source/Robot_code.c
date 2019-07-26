
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

#include <stdbool.h>
#include "fsl_ctimer.h"
/* TODO: insert other definitions and declarations here. */


#define CTIMER CTIMER1		// Timer 0
#define CTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_AsyncApbClk)

#define LM1 kCTIMER_Match_3		//J2[15]
#define LM2 kCTIMER_Match_1		//J2[3]


#define RM1 kCTIMER_Match_2		//J1[16]
#define RM2 kCTIMER_Match_0		//J1[19]







#define DEMO_USART USART0
#define DEMO_USART_IRQHandler FLEXCOMM0_IRQHandler
#define DEMO_USART_IRQn FLEXCOMM0_IRQn
/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)
#define USART_NVIC_PRIO 5
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void uart_task(void *pvParameters);
static void Drive_task(void *pvParameters);


void MotorsSetup();
void Move();
void Turn_SlowLeft();
void Turn_SlowRight();
void Turn_Left();
void Turn_Right();
void Stop();
void Reverse();


uint8_t background_buffer[32];
uint8_t recv_buffer[1];

usart_rtos_handle_t handle;
struct _usart_handle t_handle;

struct rtos_usart_config usart_config = {
    .baudrate    = 115200,
    .parity      = kUSART_ParityDisabled,
    .stopbits    = kUSART_OneStopBit,
    .buffer      = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

xQueueHandle queue1= NULL;

/*
 * @brief   Application entry point.
 */
int main(void) {



	CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
	SYSCON->ASYNCAPBCTRL = 1;
	CLOCK_AttachClk(kFRO12M_to_ASYNC_APB);
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();


  // MotorsSetup();

    ctimer_config_t config;
    		 uint32_t srcClock_Hz;
    		 srcClock_Hz = CLOCK_GetFreq(kCLOCK_BusClk);



    	    CTIMER_GetDefaultConfig(&config);


    	    CTIMER_Init(CTIMER, &config);

    	    CTIMER_SetupPwm(CTIMER,LM1,50,20000,srcClock_Hz,NULL);
    	    CTIMER_SetupPwm(CTIMER,LM2,50,20000,srcClock_Hz,NULL);
    	    CTIMER_SetupPwm(CTIMER,RM1,50,20000,srcClock_Hz,NULL);
    	    CTIMER_SetupPwm(CTIMER,RM2,50,20000,srcClock_Hz,NULL);
    	    CTIMER_StartTimer(CTIMER);


    	  	CTIMER_UpdatePwmDutycycle(CTIMER, LM1,80);
    	  	CTIMER_UpdatePwmDutycycle(CTIMER, LM2,80);
    	  	CTIMER_UpdatePwmDutycycle(CTIMER, LM1,100);

    	  	CTIMER_UpdatePwmDutycycle(CTIMER, RM1,50);
    	  	CTIMER_UpdatePwmDutycycle(CTIMER, RM2,80);


    queue1=xQueueCreate(1,sizeof(char));
    if (xTaskCreate(uart_task, "Uart_task", configMINIMAL_STACK_SIZE + 10, NULL, 2, NULL) != pdPASS)
      {
          PRINTF("Task creation failed!.\r\n");
          while (1)
              ;
      }
    if (xTaskCreate(Drive_task, "Robot_driving_task", configMINIMAL_STACK_SIZE + 10, NULL,2, NULL) != pdPASS)
             {
                 PRINTF("Task creation failed!.\r\n");
                 while (1)
                     ;
             }

      vTaskStartScheduler();
      for (;;)
          ;
  }

  /*!
   * @brief Task responsible for loopback.
   */
  static void uart_task(void *pvParameters)
  {
      char send;
      size_t n            = 0;
      usart_config.srcclk = BOARD_DEBUG_UART_CLK_FREQ;
      usart_config.base   = DEMO_USART;

      NVIC_SetPriority(DEMO_USART_IRQn, USART_NVIC_PRIO);

      USART_RTOS_Init(&handle, &t_handle, &usart_config);

      /* Receive user input and send it back to terminal. */
      while(1)
      {
          USART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
          if (n > 0)
          {
        		send=recv_buffer[0];
        		xQueueSend(queue1,&send,10);
          }
      }

  }



  static void Drive_task(void *pvParameters)
  {
	  char recv;

	  	while(1){
	  		xQueueReceive(queue1,&recv,10);
	  		if(recv=='M')
	  		{
	  			Move();
	  			printf("moving\n");
	  			recv='n';
	  		}
	  		else if(recv=='L')
	  		{
	  			Turn_Left();
	  			printf("left\n");
	  			recv='n';
	  		}
	  		else if(recv=='R')
	  		{
	  			Turn_Right();
	  			printf("right\n");
	  			recv='n';
	  		}
	  		else if(recv=='S')
	  		{

	  			Stop();
	  			printf("stop");
	  			recv='n';

	  		}
	  		else if(recv=='B')
	  		{
	  			 Reverse();
	  			 printf("reverse\n");
	  			recv='n';
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

	    CTIMER_SetupPwm(CTIMER,LM1,0,20000,srcClock_Hz,NULL);
	    CTIMER_SetupPwm(CTIMER,LM2,0,20000,srcClock_Hz,NULL);
	    CTIMER_SetupPwm(CTIMER,RM1,0,20000,srcClock_Hz,NULL);
	    CTIMER_SetupPwm(CTIMER,RM2,0,20000,srcClock_Hz,NULL);
	    CTIMER_StartTimer(CTIMER);
  }


  void Move()
  {
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 80);
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM2, 0);


  	CTIMER_UpdatePwmDutycycle(CTIMER, RM1, 0);
  	CTIMER_UpdatePwmDutycycle(CTIMER, RM2, 80);

  }


  void Turn_SlowLeft()
  {
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 75);
  		CTIMER_UpdatePwmDutycycle(CTIMER, LM2, 0);


  		CTIMER_UpdatePwmDutycycle(CTIMER, RM1, 0);
  		CTIMER_UpdatePwmDutycycle(CTIMER, RM2, 75);

  }
  void Turn_SlowRight()
  {
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 75);
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM2, 0);


  		CTIMER_UpdatePwmDutycycle(CTIMER, RM1, 0);
  		CTIMER_UpdatePwmDutycycle(CTIMER, RM2, 75);
  }
  void Turn_Left()
  {
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 100);
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM2, 0);


  	CTIMER_UpdatePwmDutycycle(CTIMER, RM1, 0);
  	CTIMER_UpdatePwmDutycycle(CTIMER, RM2, 0);
  }
  void Turn_Right()
  {
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM2, 0);


  	CTIMER_UpdatePwmDutycycle(CTIMER, RM1, 0);
  	CTIMER_UpdatePwmDutycycle(CTIMER, RM2, 100);

  }

  /* Api is used to stop the robot */

  void Stop(){
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM2, 0);


  	CTIMER_UpdatePwmDutycycle(CTIMER, RM1, 0);
  	CTIMER_UpdatePwmDutycycle(CTIMER, RM2, 0);
  }



  void Reverse(){

  	CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
  	CTIMER_UpdatePwmDutycycle(CTIMER, LM2, 100);


  	CTIMER_UpdatePwmDutycycle(CTIMER, RM1, 100);
  	CTIMER_UpdatePwmDutycycle(CTIMER, RM2, 0);

  }














