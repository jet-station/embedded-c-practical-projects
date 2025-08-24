/*----------------------------------------------------------------------------------*/
/* Copyright (C) 2025 Sigma eLabs, <https:/github.com/Sigma-eLabs>.                 */
/*                                                                                  */
/*                               S i g m a  e L a b s                               */
/*                    -------------------------------------------                   */
/*                    Embedded Software Education & Research Labs                   */
/*                                                                                  */
/* This is an opened source software for studying and researching purposes with     */
/* a non-comercial license.                                                         */
/*                                                                                  */
/* However, redistributions in source code must retain this top-level               */
/* comment block, to respect the contributions of the Sigma eLabs's authors.        */
/*                                                                                  */
/* Contact information:                                                             */
/*  <thienaiho95@gmail.com>                                                         */
/*  <https:/www.linkedin.com/in/thien-ai-ho>                                        */
/*----------------------------------------------------------------------------------*/

#include "bsp.h" /* Board Support Package for EK-TM4C123GXL */

#define BLINK_TIMES (5U)

typedef enum {
	BLINKY_INITIAL,
	BLINKY_OFF,
	BLINKY_ON,
	BLINKY_STOP
} BlinkyState;

/* Initialize blinky job state with !requested  */
BlinkyJobState volatile blinkyJobStat = JOB_FINISHED;

/* state of LED blinky */
BlinkyState blinkyState = BLINKY_INITIAL;

int main(void) {
	
	BSP_init();
	
	/* endless loop */
    while (1) {

		if (blinkyJobStat == JOB_REQUESTED) {	/* job was pressed? */
			
			/* Blink red LED for a while */
			
			static uint8_t blinkCounter; 	/* blink counter for LED */
			static uint32_t start;	/* start timestamp */
			
			switch (blinkyState) {
				case BLINKY_INITIAL: {
					BSP_setRedledState(LED_ON);
					blinkyState = BLINKY_ON;
					blinkCounter = BLINK_TIMES;
					start = BSP_getTickCounter();
					break;
				}
				case BLINKY_ON: {
					if ((BSP_getTickCounter() - start) > (BSP_TICKS_PER_SEC / 2U)) {
						BSP_setRedledState(LED_OFF);
						blinkyState = BLINKY_OFF;
						start = BSP_getTickCounter();
					}
					break;
				}
				case BLINKY_OFF: {
					if ((BSP_getTickCounter() - start) > (BSP_TICKS_PER_SEC / 2U)) {
						
						/* decrease counter for every one ON-OFF cycle of LED */
						--blinkCounter;
						if (blinkCounter != 0U) { /* continue to blink the LED? */
							BSP_setRedledState(LED_ON);
							blinkyState = BLINKY_ON;
							start = BSP_getTickCounter();
							
						}
						else {
							blinkyState = BLINKY_STOP;
						}
					}
					break;
				}
				case BLINKY_STOP: {
						/* reset the state of UsrSw1 button */
						blinkyJobStat = JOB_FINISHED;	/* job is done */
						blinkyState = BLINKY_INITIAL;	/* reset the blinky state */
					break;
				}
				default: {
					break;
				}
			}
		}
	}

    return 0;
}