#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54114_cm4.h"
#include "fsl_debug_console.h"


/* TODO: insert other include files here. */

#include "fsl_ctimer.h"





/* TODO: insert other definitions and declarations here. */
#define CTIMER CTIMER0                 /* Timer 0 */
#define J2_7 kCTIMER_Match_1
#define J2_9 kCTIMER_Match_0/* Match output 1 */
//#define J1_12 kCTIMER_Match_3
//#define J1_10 kCTIMER_Match_2


#define CTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)




int main(void) {
	 ctimer_config_t config;
	 uint32_t srcClock_Hz;

	 //CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

	    /* Enable the asynchronous bridge */
	    //SYSCON->ASYNCAPBCTRL = 1;

	    /* Use 12 MHz clock for some of the Ctimers */
	    //CLOCK_AttachClk(kFRO12M_to_ASYNC_APB);


  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    srcClock_Hz = CLOCK_GetFreq(kCLOCK_BusClk);

    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER, &config);
    CTIMER_SetupPwm(CTIMER,J2_7,50,20000,srcClock_Hz,NULL);
    CTIMER_SetupPwm(CTIMER,J2_9,50,20000,srcClock_Hz,NULL);
    //CTIMER_SetupPwm(CTIMER,J1_12,50,20000,srcClock_Hz,NULL);
    //CTIMER_SetupPwm(CTIMER,J1_10,50,20000,srcClock_Hz,NULL);

    CTIMER_StartTimer(CTIMER);

   // CTIMER_UpdatePwmDutycycle(CTIMER,L1,80);
    CTIMER_UpdatePwmDutycycle(CTIMER,J2_7,0);
    CTIMER_UpdatePwmDutycycle(CTIMER,J2_7,100);

    CTIMER_UpdatePwmDutycycle(CTIMER,J2_9,0);
    CTIMER_UpdatePwmDutycycle(CTIMER,J2_9,100);

    //CTIMER_UpdatePwmDutycycle(CTIMER,J1_12,0);
    //CTIMER_UpdatePwmDutycycle(CTIMER,J1_12,100);

    //CTIMER_UpdatePwmDutycycle(CTIMER,J1_12,0);
    //CTIMER_UpdatePwmDutycycle(CTIMER,J1_12,100);

    while(1);

}
