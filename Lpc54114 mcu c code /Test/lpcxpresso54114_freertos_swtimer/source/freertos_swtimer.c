/* Standard includes. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"


/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"


#include "pin_mux.h"
#include <stdbool.h>


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define pdTICKS_TO_MS( xTicks )    ( ( ( TickType_t ) ( xTicks ) * 1000u ) / configTICK_RATE_HZ )

/* The software timer period. */
#define SW_TIMER_PERIOD_MS (1000 / portTICK_PERIOD_MS)



/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/* The callback function. */


static void SwTimerCallback(TimerHandle_t xTimer);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    TimerHandle_t SwTimerHandle = NULL;

    /* Init board hardware. */
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();
    SystemCoreClockUpdate();
    /* Create the software timer. */
    SwTimerHandle = xTimerCreate("SwTimer",          /* Text name. */
                                 SW_TIMER_PERIOD_MS, /* Timer period. */
                                 pdTRUE,             /* Enable auto reload. */
                                 0,                  /* ID is not used. */
                                 SwTimerCallback);   /* The callback function. */


    /* Start timer. */
    xTimerStart(SwTimerHandle, 0);
   /* Start scheduling. */
    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Software timer callback.
 */
static void SwTimerCallback(TimerHandle_t xTimer)
{
	TickType_t xTimerPeriod;

	xTimerPeriod=xTimerGetPeriod( xTimer );

	float j = pdTICKS_TO_MS(xTimerPeriod);


    PRINTF("Tick.%d\r\n",j);
}
