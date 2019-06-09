/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    freeRtos - main.c
 * @brief   Application entry point.
 */
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

#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)

/* TODO: insert other definitions and declarations here.


#define CTIMER CTIMER1		/* Timer 1
#define LEFT_MOTOR_1 kCTIMER_Match_0		//J2[9]
#define LEFT_MOTOR_2 kCTIMER_Match_1		//J2[7]
#define RIGHT_MOTOR_1 kCTIMER_Match_3		//J2[16]
#define RIGHT_MOTOR_2 kCTIMER_Match_2		//J1[10]




/* Match output 1
#define CTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_AsyncApbClk)

*/
static void hello_task(void *pvParameters)
{
	while(1){

		 PRINTF("Hello world.\r\n");
	//CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR_1, 10);
	//CTIMER_UpdatePwmDutycycle(CTIMER, LEFT_MOTOR_2, 50);
	vTaskSuspend(NULL);
	}
}


/*void MotorsSetup()
{
	ctimer_config_t config;

	CTIMER_GetDefaultConfig(&config);


	    CTIMER_Init(CTIMER, &config);


	    CTIMER_SetupPwm(CTIMER,LEFT_MOTOR_1,0,24000U,CTIMER_CLK_FREQ,false);
	    CTIMER_SetupPwm(CTIMER,LEFT_MOTOR_2,0,24000U,CTIMER_CLK_FREQ,false);
	    CTIMER_SetupPwm(CTIMER,RIGHT_MOTOR_1,0,24000U,CTIMER_CLK_FREQ,false);
	    CTIMER_SetupPwm(CTIMER,RIGHT_MOTOR_2,0,24000U,CTIMER_CLK_FREQ,false);
	    CTIMER_StartTimer(CTIMER);
}*/


/*
 * @brief   Application entry point.
 */
int main(void) {


	//SYSCON->ASYNCAPBCTRL = 1;
	//CLOCK_AttachClk(kFRO12M_to_ASYNC_APB);
	  CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);


  	/* Init board hardware. */
    BOARD_InitBootPins();

    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();
    //MotorsSetup();


    if (xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, 0, NULL) != pdPASS)
       {
           PRINTF("Task creation failed!.\r\n");
           while (1)
               ;
       }
    vTaskStartScheduler();
       for (;;)
           ;
}
