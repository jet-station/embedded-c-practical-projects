/*****************************************************************************
 * @file      bsp_ektm4c123gxl.c
 * @author    Jet Station
 * @brief     Board Support Package for EK-TM4C123GXL Development Board
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
 
#include "bsp_ektm4c123gxl.h"

static void BSP_Ektm4c123gxl_turnLedRedOn(void)
{
	/* TODO: Implement actual code */
}

static void BSP_Ektm4c123gxl_turnLedGreenOn(void)
{
	/* TODO: Implement actual code */
}

static void BSP_Ektm4c123gxl_turnLedBlueOn(void)
{
	/* TODO: Implement actual code */
}

void BSP_Ektm4c123gxl_turnOnBoardLedsOn(void)
{
	BSP_Ektm4c123gxl_turnLedRedOn();
	BSP_Ektm4c123gxl_turnLedGreenOn();
	BSP_Ektm4c123gxl_turnLedBlueOn();
}
