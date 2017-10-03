/*
 * timerISR.c
 *
 *  Created on: 
 *      Author: 
 */
#include "System.h"
#include "io.h"
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
	alt_u32* pCount = (alt_u32*)context;
	/* clear the interrupt */
	IOWR(INTERVAL_TIMER_BASE, 0, 1); //Any value written to the status register clears the interrupt

	/* Act upon the interrupt */
	*pCount = *pCount + 1;			//Incrementing the counter

	/* Display count value 
	 * (Is this the best place to do this? 
	 * Why or why not?) 
	 */
	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *pCount);	/*Writing the count value to the HEX display,
													  best practice says to do as little as necessary in an ISR,
													  so the display should be done in the main program since interrupts could
													  be disabled by a high priority function*/
	return;
}



