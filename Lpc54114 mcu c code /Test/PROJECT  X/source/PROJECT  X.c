#include <stdio.h>

#include "board.h"

#include "peripherals.h"

#include "pin_mux.h"

#include "clock_config.h"

#include "fsl_debug_console.h"

#include "LPC54114_cm4.h"

/* TODO: insert other include files here. */



/* FreeRTOS kernel includes. */

#include "FreeRTOS.h"

#include "task.h"





/* TODO: insert other definitions and declarations here. */











/* Task priorities. */

#define my_task_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************

 * Prototypes

 ******************************************************************************/

static void my_task(void *pvParameters);





/*!

 * @brief Task responsible for printing of "Hello world." message.

 */

static void my_task(void *pvParameters)

{

    for (;;)

    {

        PRINTF("Hello World!\r\n");

        vTaskSuspend(NULL);

    }

}







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



    if (xTaskCreate(my_task, "my_task", configMINIMAL_STACK_SIZE + 10, NULL, my_task_PRIORITY, NULL) != pdPASS)

    {

        PRINTF("Task creation failed!.\r\n");

        while (1)

            ;

    }

    vTaskStartScheduler();



    /* Enter an infinite loop, just incrementing a counter. */

    while(1) {



    }

    return 0 ;

}
