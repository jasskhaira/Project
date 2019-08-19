
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
#include "fsl_sctimer.h"

#define SCTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define DEMO_FIRST_SCTIMER_OUT kSCTIMER_Out_7             //pwm signal output for motor1 left
#define DEMO_SECOND_SCTIMER_OUT kSCTIMER_Out_5             //pwm signal output2 for motor 1// left
#define DEMO_THIRD_SCTIMER_OUT kSCTIMER_Out_4               //pwm signal output for motor 2 right
#define DEMO_FOURTH_SCTIMER_OUT kSCTIMER_Out_2				//pwm signal output2 for motor 2 right

#define DEMO_USART USART0
#define DEMO_USART_IRQHandler FLEXCOMM0_IRQHandler
#define DEMO_USART_IRQn FLEXCOMM0_IRQn

#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)
#define USART_NVIC_PRIO 5

uint8_t background_buffer[100];
uint8_t recv_buffer[1];

xQueueHandle queue1= NULL;

usart_rtos_handle_t handle;
struct _usart_handle t_handle;


sctimer_config_t sctimerInfo;
sctimer_pwm_signal_param_t pwm1,pwm2,pwm3,pwm4;
uint32_t eventnumber1,eventnumber2,eventnumber3,eventnumber4;
uint32_t sctimerClock;


struct rtos_usart_config usart_config = {
    .baudrate    = 9600,
    .parity      = kUSART_ParityDisabled,
    .stopbits    = kUSART_OneStopBit,
    .buffer      = background_buffer,
    .buffer_size = sizeof(background_buffer),
};




static void DataTask(void *pvParameters)
 {

	 uint8_t data;
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
        	 data=recv_buffer[0];
       		xQueueSend(queue1,&data,10);
         }
         }
     }





static void MotorTask(void *pvParameters)
{
	 uint8_t recv;

	  	while(1)
	  	{
	  		xQueueReceive(queue1,&recv,10);
	  		if(recv=='g')
	  		{
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FIRST_SCTIMER_OUT, 80, eventnumber1);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_SECOND_SCTIMER_OUT,1, eventnumber2);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_THIRD_SCTIMER_OUT, 80, eventnumber3);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FOURTH_SCTIMER_OUT,1, eventnumber4);
	  			printf("m");

	  		}
	  		if(recv=='r')
	  		{
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FIRST_SCTIMER_OUT, 80, eventnumber1);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_SECOND_SCTIMER_OUT,1, eventnumber2);
	  		    SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_THIRD_SCTIMER_OUT, 1, eventnumber3);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FOURTH_SCTIMER_OUT,1, eventnumber4);
	  			printf("r");

	  		}
	  		if(recv=='l')
	  		{
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FIRST_SCTIMER_OUT, 1, eventnumber1);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_SECOND_SCTIMER_OUT,1, eventnumber2);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_THIRD_SCTIMER_OUT, 80, eventnumber3);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FOURTH_SCTIMER_OUT,1, eventnumber4);
	  			printf("l");

	  		}
	  		if(recv=='s')
	  		{

	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FIRST_SCTIMER_OUT, 1, eventnumber1);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_SECOND_SCTIMER_OUT,1, eventnumber2);
	  		    SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_THIRD_SCTIMER_OUT, 1, eventnumber3);
	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FOURTH_SCTIMER_OUT,1, eventnumber4);
	  			printf("s");
	  		}
	  		if(recv=='b')
	  		{

	  			SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FIRST_SCTIMER_OUT, 1, eventnumber1);
	  		    SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_SECOND_SCTIMER_OUT, 80, eventnumber2);
	  		    SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_THIRD_SCTIMER_OUT, 1, eventnumber3);
	  		    SCTIMER_UpdatePwmDutycycle(SCT0, DEMO_FOURTH_SCTIMER_OUT,80, eventnumber4);
	  		    printf("b");
	  		}

	  		recv='n';
	  		}
}



int main(void)
{




		/* Init board hardware. */



	  BOARD_InitBootPins();
	  BOARD_InitBootClocks();
	  BOARD_InitBootPeripherals();

		/* Init FSL debug console. */


	  BOARD_InitDebugConsole();

	  queue1=xQueueCreate(1,sizeof(uint8_t));
	  sctimerClock = SCTIMER_CLK_FREQ;

	  SCTIMER_GetDefaultConfig(&sctimerInfo);

		  /* Initialize SCTimer module */
	  SCTIMER_Init(SCT0, &sctimerInfo);

      /* Configure first PWM with frequency 24kHZ from first output */
      pwm1.output = DEMO_FIRST_SCTIMER_OUT;
      pwm1.level = kSCTIMER_HighTrue;
      pwm1.dutyCyclePercent = 1;
      if (SCTIMER_SetupPwm(SCT0, &pwm1, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &eventnumber1) == kStatus_Fail)
      {
          return -1;
      }

      /* Configure second PWM with different duty cycle but same frequency as before */
      pwm2.output = DEMO_SECOND_SCTIMER_OUT;
      pwm2.level = kSCTIMER_HighTrue;
      pwm2.dutyCyclePercent = 1;
      if (SCTIMER_SetupPwm(SCT0, &pwm2, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &eventnumber2) == kStatus_Fail)
      {
          return -1;
      }

      pwm3.output = DEMO_THIRD_SCTIMER_OUT;
      pwm3.level = kSCTIMER_HighTrue;
      pwm3.dutyCyclePercent = 1;
      if (SCTIMER_SetupPwm(SCT0, &pwm3, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &eventnumber3) == kStatus_Fail)
      {
          return -1;
      }

      /* Configure second PWM with different duty cycle but same frequency as before */
      pwm4.output = DEMO_FOURTH_SCTIMER_OUT;
      pwm4.level = kSCTIMER_HighTrue;
      pwm4.dutyCyclePercent = 1;
      if (SCTIMER_SetupPwm(SCT0, &pwm4, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &eventnumber4) == kStatus_Fail)
      {
          return -1;
      }

      /* Start the timer */
      SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L);



      if (xTaskCreate(MotorTask, "MotorTask", configMINIMAL_STACK_SIZE + 10, NULL,1,NULL) != pdPASS)
                     {
                         PRINTF("Task creation failed!.\r\n");
                         while (1)
                             ;
                     }

      if (xTaskCreate(DataTask, "DataTask", configMINIMAL_STACK_SIZE + 10, NULL,1,NULL) != pdPASS)
             {
                 PRINTF("Task creation failed!.\r\n");
                 while (1)
                     ;
             }


      vTaskStartScheduler();
              for (;;)
                  ;

}


