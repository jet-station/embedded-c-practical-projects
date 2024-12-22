/*----------------------------------------------------------------------------------*/
/* Copyright (C) 2024 Sigma eLabs, <https:/github.com/Sigma-eLabs>.                 */
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

/* User button switch 1 pressed signal, initialized with Released state */
ButtonState volatile usrSw1State = BUTTON_RELEASED;

int main(void) {
	
	BSP_init();
	
	/* endless loop */
    while (1) {
		if (usrSw1State == BUTTON_PRESSED) {	/* UsrSw1 button was pressed */
			/* Blink red LED for a while */	
			for (uint8_t iter = 5U; iter != 0U; --iter) {
				BSP_setRedledState(LED_ON); 	/* turn red LED on */
				BSP_delay(2000000U); 	/* delay for 2000000 interations */
				BSP_setRedledState(LED_OFF);	/* turn red LED off */
				BSP_delay(1000000U); 	/* delay for 1000000 interations */
			}
			
			/* reset the state of UsrSw1 button */
			usrSw1State = BUTTON_RELEASED;
		}
    }

    return 0;
}