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
	volatile int * interval_timer_ptr = (int *)INTERVAL_TIMER_BASE;		//Pointer to Timer Base Address
	static int count=0;
	int * count_ptr;

	/* clear the interrupt */
	*(interval_timer_ptr)=0x0;

	/* Act upon the interrupt */
	count++;
	*count_ptr=count;

	/* Display count value 
	 * (Is this the best place to do this? 
	 * Why or why not?) 
	 */
	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *count_ptr);

	return;
}



