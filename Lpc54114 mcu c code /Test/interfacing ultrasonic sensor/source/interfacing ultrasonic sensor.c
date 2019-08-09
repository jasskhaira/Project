
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





//static void Front_Ultrasonic_Task(void *pvParameters);
static void Rear_Ultrasonic_Task(void *pvParameters);




int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */

    ctimer_config_t config;

	 CTIMER_GetDefaultConfig(&config);


	 CTIMER_Init(CTIMER, &config);
	 CTIMER_Init(CTIMER1, &config);
	/* if (xTaskCreate(Front_Ultrasonic_Task, "Ultrasonic_Task", configMINIMAL_STACK_SIZE + 10, NULL,2, NULL) != pdPASS)
	                {
	                    PRINTF("Task creation failed!.\r\n");
	                    while (1)
	                        ;
	                }
*/
	 if (xTaskCreate(Rear_Ultrasonic_Task, "Ultrasonic_Task2", configMINIMAL_STACK_SIZE + 10, NULL,2, NULL) != pdPASS)
		                {
		                    PRINTF("Task creation failed!.\r\n");
		                    while (1)
		                        ;
		                }



	 vTaskStartScheduler();
	         for (;;);


}
/*

static void Front_Ultrasonic_Task(void *pvParameters)
{

	float Front_time,Front_distance;

    while(1)


	{
		GPIO_PinWrite(BOARD_Front_trig_GPIO,BOARD_Front_trig_PORT,BOARD_Front_trig_PIN,1);
		vTaskDelay(10);
		GPIO_PinWrite(BOARD_Front_trig_GPIO,BOARD_Front_trig_PORT,BOARD_Front_trig_PIN,0);

		while(GPIO_PinRead(BOARD_Front_echo_GPIO,BOARD_Front_echo_PORT,BOARD_Front_echo_PIN)==0);

		CTIMER_StartTimer(CTIMER);

		while(GPIO_PinRead(BOARD_Front_echo_GPIO,BOARD_Front_echo_PORT,BOARD_Front_echo_PIN)==1);
		CTIMER_StopTimer(CTIMER);

		Front_time= CTIMER_GetTimerCountValue(CTIMER);

		Front_distance =(0.0343*(Front_time/96))/2;

		printf("Front Distance = %lf\n",Front_distance);

		CTIMER_Reset(CTIMER);


	}

}

*/

static void Rear_Ultrasonic_Task(void *pvParameters)
{
	float Rear_time,Rear_distance,Front_time,Front_distance;


    while(1)


	{
		GPIO_PinWrite(BOARD_Rear_trig_GPIO,BOARD_Rear_trig_PORT,BOARD_Rear_trig_PIN,1);
		vTaskDelay(10);
		GPIO_PinWrite(BOARD_Rear_trig_GPIO,BOARD_Rear_trig_PORT,BOARD_Rear_trig_PIN,0);

		while(GPIO_PinRead(BOARD_Rear_echo_GPIO,BOARD_Rear_echo_PORT,BOARD_Rear_echo_PIN)==0);

		CTIMER_StartTimer(CTIMER1);

		while(GPIO_PinRead(BOARD_Rear_echo_GPIO,BOARD_Rear_echo_PORT,BOARD_Rear_echo_PIN)==1);
		CTIMER_StopTimer(CTIMER1);

		Rear_time= CTIMER_GetTimerCountValue(CTIMER1);
		Rear_time=Rear_time/96;
		Rear_distance =(0.0343*Rear_time)/2;

		printf("Rear Distance = %lf\n",Rear_distance);

		CTIMER_Reset(CTIMER1);

		GPIO_PinWrite(BOARD_Front_trig_GPIO,BOARD_Front_trig_PORT,BOARD_Front_trig_PIN,1);
		vTaskDelay(10);
		GPIO_PinWrite(BOARD_Front_trig_GPIO,BOARD_Front_trig_PORT,BOARD_Front_trig_PIN,0);

		while(GPIO_PinRead(BOARD_Front_echo_GPIO,BOARD_Front_echo_PORT,BOARD_Front_echo_PIN)==0);

		CTIMER_StartTimer(CTIMER);

		while(GPIO_PinRead(BOARD_Front_echo_GPIO,BOARD_Front_echo_PORT,BOARD_Front_echo_PIN)==1);
		CTIMER_StopTimer(CTIMER);

		Front_time= CTIMER_GetTimerCountValue(CTIMER);

		Front_distance =(0.0343*(Front_time/96))/2;

		printf("Front Distance = %lf\n",Front_distance);

		CTIMER_Reset(CTIMER);


	}

}


