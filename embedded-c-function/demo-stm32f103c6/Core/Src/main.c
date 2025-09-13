
#include <stdbool.h> /* Standard bool data types */
#include <stdint.h> /* Standard integer data types */

/* Global variable declaration section start ----------------------------------------------*/

uint32_t g_counter_u32 = 0U;
uint32_t g_counterThres_u32 = 0U;

/* Global variable declaration section end ------------------------------------------------*/


/* Function declaration section start -----------------------------------------------------*/

/**
  * @brief  Count up counter
  * @param  None
  * @retval None
  */
static void Counter_countUp(void);

/**
  * @brief  Get counter value
  * @param  None
  * @retval uint32_t
  */
uint32_t Counter_getValue(void);

/**
  * @brief  Reset counter value
  * @param  None
  * @retval None
  */
void Counter_resetCounter(void);

/**
  * @brief  Set counter threshold
  * @param  uint32_t
  * @retval None
  */
void Counter_setCounterThres(uint32_t thresHold_u32);

/**
  * @brief  Get counter threshold
  * @param  None
  * @retval uint32_t
  */
uint32_t Counter_getCounterThres(void);

/**
  * @brief  Check if counter is overed threshold
  * @param  uint32_t
  * @retval bool
  */
bool Counter_isOvered(uint32_t thresHold_u32);

/**
  * @brief  Turn on-board LED on
  * @param  None
  * @retval None
  */
void BSP_setOnBoardLedOn(void);

/**
  * @brief  Turn on-board LED off
  * @param  None
  * @retval None
  */
void BSP_setOnBoardLedOff(void);

/* Function declaration section end -------------------------------------------------------*/

/* Function definition section start ------------------------------------------------------*/

/**
  * @brief  Count up counter
  * @param  None
  * @retval None
  */
static void Counter_countUp(void) 
{
	g_counter_u32++;
}

/**
  * @brief  Get counter value
  * @param  None
  * @retval uint32_t
  */
uint32_t Counter_getValue(void)
{
	return g_counter_u32;
}


/**
  * @brief  Reset counter value
  * @param  None
  * @retval None
  */
void Counter_resetCounter(void) 
{
	g_counter_u32 = 0U;
}

/**
  * @brief  Set counter threshold
  * @param  uint32_t
  * @retval None
  */
void Counter_setCounterThres(uint32_t thresHold_u32)
{
	g_counterThres_u32 = thresHold_u32;
}

/**
  * @brief  Get counter threshold
  * @param  None
  * @retval uint32_t
  */
uint32_t Counter_getCounterThres(void) 
{
	return g_counterThres_u32;
}

/**
  * @brief  Check if counter is overed threshold
  * @param  uint32_t
  * @retval bool
  */
bool Counter_isOvered(uint32_t thresHold_u32)
{
	bool l_isOvered_b = false;
	uint32_t l_counter_u32 = 0U;
	
	/* get counter value */
	l_counter_u32 = Counter_getValue();
	
	if (l_counter_u32 >= thresHold_u32)
	{
		l_isOvered_b = true;
	}
	else
	{
		l_isOvered_b = false;
	}
	
	return l_isOvered_b;
}

/**
  * @brief  Turn on-board LED on
  * @param  None
  * @retval None
  */
void BSP_setOnBoardLedOn(void) 
{
	/* Todo: Implement actual code to turn on-board LED on */
}

/**
  * @brief  Turn on-board LED off
  * @param  None
  * @retval None
  */
void BSP_setOnBoardLedOff(void)
{
	/* Todo: Implement actual code to turn on-board LED off */
}

/* Function definition section end -------------------------------------------------------*/

/* main function definition start --------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* local variable */
	bool l_isCounterOvered_b = false;
	uint32_t l_counterThres_u32 = 0U;
	
	/* initialization */
	Counter_resetCounter();
	Counter_setCounterThres(1000U);
	BSP_setOnBoardLedOff();
	
	/* infinite loop */
	while (1)
	{
		/* count up every loop iteration */
		Counter_countUp();
		
		/* get current counter threshold */
		l_counterThres_u32 = Counter_getCounterThres();
		
		l_isCounterOvered_b = Counter_isOvered(l_counterThres_u32);
		
		/* check if counter expired */
		if (true == l_isCounterOvered_b)
		{
			
			/* reset counter when detecting it expired */
			Counter_resetCounter();
			
			/* indicate counter expired for user */
			BSP_setOnBoardLedOn();
		}
		else
		{
			/* indicate counter is counting up for user */
			BSP_setOnBoardLedOff();
		}
	}

	return 0;
}

/* main function definition end ----------------------------------------------------------*/