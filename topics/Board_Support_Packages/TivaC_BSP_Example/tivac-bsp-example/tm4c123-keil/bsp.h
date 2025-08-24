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

/* Board Support Package APIs for EK-TM4C123GXL */
#ifndef __BSP_H__
#define __BSP_H__

#include <stdint.h> /* standard integers library */

/* on-board LEDs */
#define LED_RED   	(1U << 1)	/* PF1 - red LED */
#define LED_BLUE   	(1U << 2)	/* PF2 - blue LED */
#define LED_GREEN   (1U << 3) 	/* PF3 - green LED */

/* on-board Buttons */
#define USR_SW1		(1U << 4) 	/* PF4 - User button switch 1 */
#define USR_SW2		(1U << 0)	/* PF0 - User button switch 2 */

/* state type of on-board LEDs */
typedef enum {
	LED_OFF = 0U,
	LED_ON
} LedState;

/* state type of on-board Buttons */
typedef enum {
	BUTTON_RELEASED = 0U,
	BUTTON_PRESSED
} ButtonState;

/* global variable to be used in this module */
extern ButtonState volatile usrSw1State;

/* Board initialization */
void BSP_init(void);

/* set state of on-board Red LEDs */
void BSP_setRedledState(LedState ledState);

/* set state of on-board Blue LEDs */
void BSP_setBlueledState(LedState ledState);

/* set state of on-board Green LEDs */
void BSP_setGreenledState(LedState ledState);

/* delay for interations  */
void BSP_delay(uint32_t volatile iter);

#endif /* __BSP_H__ */