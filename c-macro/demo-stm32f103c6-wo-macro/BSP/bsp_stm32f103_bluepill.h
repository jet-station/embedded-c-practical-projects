/*****************************************************************************
 * @file      bsp_stm32f103_bluepill.h
 * @author    Jet Station
 * @brief     Board Support Package for STM32F103 BluePill Development Board
 * @date      [2025-09-20]
 *
 * Contact:
 *   @website   https://jet-station.github.io/
 *   @github    https://github.com/jet-station
 *   @linkedin  https://www.linkedin.com/in/thien-ai-ho/
 *   @email     thienaiho95@gmail.com
 *
 * @copyright  Copyright (c) 2025 Jet Station. All rights reserved.
 *****************************************************************************/

typedef enum
{
	BAD = 0U,
	GOOD
} BoardStatus;

void BSP_Stm32f103BluePill_turnOnBoardLedsOn(void);
