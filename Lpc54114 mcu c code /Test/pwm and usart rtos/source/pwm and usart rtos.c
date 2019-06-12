#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54114_cm4.h"
#include "fsl_debug_console.h"
#include "fsl_usart_freertos.h"
#include "fsl_usart.h"
/* TODO: insert other include files here. */


/* TODO: insert other definitions and declarations here. */
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

/*******************************************************************************
 * Code
 ******************************************************************************/
const char *to_send = "FreeRTOS USART driver example!\r\n";
const char *send_buffer_overrun = "\r\nRing buffer overrun!\r\n";
uint8_t background_buffer[32];
uint8_t recv_buffer[4];

usart_rtos_handle_t handle;
struct _usart_handle t_handle;

struct rtos_usart_config usart_config = {
		.base = DEMO_USART,
		.srcclk = BOARD_DEBUG_UART_CLK_FREQ,
		.baudrate = 115200,
    .parity = kUSART_ParityDisabled,
    .stopbits = kUSART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
};
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

    USART_RTOS_Init(&handle, &t_handle, &usart_config);

    if (xTaskCreate(uart_task, "Uart_task", configMINIMAL_STACK_SIZE + 10, NULL, uart_task_PRIORITY, NULL) != pdPASS)
      {
          PRINTF("Task creation failed!.\r\n");
          while (1)
              ;
      }
      vTaskStartScheduler();
      for (;;)
          ;
  }


static void uart_task(void *pvParameters)
{

	 if (0 > USART_RTOS_Send(&handle, (uint8_t *)to_send, strlen(to_send)))
	    {
	        vTaskSuspend(NULL);
	    }
	 vTaskSuspend(NULL);

}

