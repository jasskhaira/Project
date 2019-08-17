
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54114_cm4.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


#include "fsl_ctimer.h"


#define CTIMER CTIMER0                 /* Timer 0 */
#define LM0 kCTIMER_Match_0			//	J1[19]
#define RM0 kCTIMER_Match_1			//  J2[18]
#define LM1 kCTIMER_Match_2			//  J1[16]
#define RM1 kCTIMER_Match_0			//  J2[17]

static void Object_Search();

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


int main(void) {

	SYSCON->ASYNCAPBCTRL = 1;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();


    MotorsSetup();
    if (xTaskCreate(Object_Search, "Object_Search", configMINIMAL_STACK_SIZE + 10, NULL,2,NULL) != pdPASS)
                   {
                       PRINTF("Task creation failed!.\r\n");
                       while (1)
                           ;
                   }


    return 0 ;
}

static void Object_Search()
{
	 Search();
	vTaskDelay(500);
	Stop();
	vTaskDelay(10000);


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
	 CTIMER_UpdatePwmDutycycle(CTIMER, LM0, 75);
	 CTIMER_UpdatePwmDutycycle(CTIMER, LM1, 0);
	 CTIMER_UpdatePwmDutycycle(CTIMER, RM0, 0);
	 CTIMER_UpdatePwmDutycycle(CTIMER1,RM1, 75);

}


