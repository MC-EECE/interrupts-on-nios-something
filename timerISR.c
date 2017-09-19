/*
 * timerISR.c
 *
 *  Created on: 
 *      Author: 
 */
#include "System.h"
#include "alt_types.h"
#include "HexDisplay.h"

/*****************************************************************************
 * Interval timer interrupt service routine
 *
 *
******************************************************************************/
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void timerISR(void* context)
#else
void timerISR(void * context, alt_u32 id)
#endif
{
	/* Cast context. It is important that this be declared
	 * volatile to avoid unwanted compiler optimization.
	 */

	/* clear the interrupt */

	/* Act upon the interrupt */

	/* Display count value 
	 * (Is this the best place to do this? 
	 * Why or why not?) 
	 */
	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *count_ptr);

	return;
}



