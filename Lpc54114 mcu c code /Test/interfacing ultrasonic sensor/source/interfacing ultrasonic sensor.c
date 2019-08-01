
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54114_cm4.h"
#include "fsl_debug_console.h"
#include  <math.h>
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


    portFLOAT Distance=0.0;
        portFLOAT TP=0.0;
        portFLOAT md0=0.0;

        printf("md0=%f\n",md0);
        printf("Distance=%f\n",Distance);
        printf("TP=%f\n",TP);


    ctimer_config_t config;

	 CTIMER_GetDefaultConfig(&config);

	// config.prescale=((96000000/1000000)-1);
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
	portFLOAT Distance=0.0;
	    portFLOAT TP=0.0;
	    portFLOAT md0=0.0;

	    printf("md0=%f\n",md0);
	    printf("Distance=%f\n",Distance);
	    printf("TP=%f\n",TP);


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

		printf("tp1 = %d\n",TP);


		md0=TP/96;


		printf("tp2 = %f\n",md0);


		Distance =(0.0343*md0)/2;

		printf("Distance = %lf\n",Distance);

		CTIMER_Reset(CTIMER);


	}

}

