
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54114_cm4.h"
#include "fsl_debug_console.h"


/* TODO: insert other include files here. */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


#include "fsl_ctimer.h"


/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    PRINTF("Hello World\n");


    portFLOAT Distance=0.0;
    portFLOAT TP=0.0;
    portFLOAT md0=0.0;

    printf("md0=%f\n",md0);
    printf("Distance=%f\n",Distance);
    printf("TP=%f\n",TP);

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}
