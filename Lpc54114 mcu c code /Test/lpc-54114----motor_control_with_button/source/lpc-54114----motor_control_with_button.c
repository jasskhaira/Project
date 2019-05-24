/**
 * @file    lpc-54114----motor_control_with_button.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC54114_cm4.h"
#include "fsl_debug_console.h"


/* TODO: insert other include files here. */
#include "fsl_sctimer.h"



/* TODO: insert other definitions and declarations here. */

#define SCTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
#define LEFT_MOTOR_SIGNAL1 kSCTIMER_Out_2                //D5 pin
#define LEFT_MOTOR_SIGNAL2 kSCTIMER_Out_7				//D7 pin
#define RIGHT_MOTOR_SIGNAL1 kSCTIMER_Out_4				//D10 pin
#define RIGHT_MOTOR_SIGNAL2 kSCTIMER_Out_5				//D9 pin
uint32_t eventNumberOutput;


/*
 * @brief   Application entry point.
 */
int main(void) {

	 sctimer_config_t sctimerInfo;
		    sctimer_pwm_signal_param_t pwmParam_left1,pwmParam_left2,pwmParam_right1,pwmParam_right2;
		    uint32_t event;
		    uint32_t sctimerClock;










  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    sctimerClock = SCTIMER_CLK_FREQ;

    /* Configure first PWM with frequency 24kHZ from LEFT MOTOR FIRST SIGNAL output */
        pwmParam_left1.output = LEFT_MOTOR_SIGNAL1;
        pwmParam_left1.level = kSCTIMER_HighTrue;
        pwmParam_left1.dutyCyclePercent = 20;


           /* Configure first PWM with frequency 24kHZ from LEFT MOTOR SECOND SIGNAL output */
        pwmParam_left2.output = LEFT_MOTOR_SIGNAL2;
        pwmParam_left2.level = kSCTIMER_HighTrue;
        pwmParam_left2.dutyCyclePercent = 80;

              /* Configure first PWM with frequency 24kHZ from RIGHT MOTOR FIRST SIGNAL output */
        pwmParam_right1.output = RIGHT_MOTOR_SIGNAL1;
        pwmParam_right1.level = kSCTIMER_HighTrue;
        pwmParam_right1.dutyCyclePercent = 50;

                 /* Configure first PWM with frequency 24kHZ from RIGHT MOTOR SECOND SIGNAL output */
        pwmParam_right2.output = RIGHT_MOTOR_SIGNAL2;
        pwmParam_right2.level = kSCTIMER_HighTrue;
        pwmParam_right2.dutyCyclePercent = 70;

        SCTIMER_GetDefaultConfig(&sctimerInfo);

           /* Initialize SCTimer module */
        SCTIMER_Init(SCT0, &sctimerInfo);

        SCTIMER_SetupPwm(SCT0, &pwmParam_left1, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &event);
        SCTIMER_SetupPwm(SCT0, &pwmParam_left2, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &event);

        SCTIMER_SetupPwm(SCT0, &pwmParam_right1, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &event);
        SCTIMER_SetupPwm(SCT0, &pwmParam_right2, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock, &event);

        SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L);

        SCTIMER_UpdatePwmDutycycle(SCT0,LEFT_MOTOR_SIGNAL1,90,eventNumberOutput);




    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
    }
    return 0 ;
}
