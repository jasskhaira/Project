
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


#include "fsl_ctimer.h"

#define CTIMER CTIMER0

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */




static void Ultrasonic_Task(void *pvParameters);





int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */


    ctimer_config_t config;

	 CTIMER_GetDefaultConfig(&config);
	 CTIMER_Init(CTIMER, &config);
	 if (xTaskCreate(Ultrasonic_Task, "Ultrasonic_Task", configMINIMAL_STACK_SIZE + 10, NULL,2, NULL) != pdPASS)
	                {
	                    PRINTF("Task creation failed!.\r\n");
	                    while (1)
	                        ;
	                }

	 vTaskStartScheduler();
	         for (;;)
	             ;


}


static void Ultrasonic_Task(void *pvParameters)
{
   float Distance,TP;

	while(1)


	{
		GPIO_PinWrite(BOARD_trig_GPIO,BOARD_trig_PORT,BOARD_trig_PIN,1);
		vTaskDelay(10);
		GPIO_PinWrite(BOARD_trig_GPIO,BOARD_trig_PORT,BOARD_trig_PIN,0);

		while(GPIO_PinRead(BOARD_echo_GPIO,BOARD_echo_PORT,BOARD_echo_PIN)==0);

		CTIMER_StartTimer(CTIMER);

		while(GPIO_PinRead(BOARD_echo_GPIO,BOARD_echo_PORT,BOARD_echo_PIN)==1);

		CTIMER_StopTimer(CTIMER);
		TP= CTIMER_GetTimerCountValue(CTIMER);

		Distance =(0.0343*TP)/2;

		printf("Distance = %0.2fcm\n",Distance);

		CTIMER_Reset(CTIMER);

	}

}

