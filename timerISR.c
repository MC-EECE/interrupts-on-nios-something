/*
 * timerISR.c
 *
 *  Created on: Apr 18, 2015
 *      Author: brent.horine
 */
#include "System.h"
#include "alt_types.h"
#include "HexDisplay.h"
extern alt_u8 paused;
extern alt_u8 reset;

/*****************************************************************************
 * Interval timer interrupt service routine
 *
 *
******************************************************************************/
void timerISR(void * context)
{
	volatile alt_u16 * count_ptr = (volatile alt_u16*)context;
	volatile int * interval_timer_ptr = (int *) INTERVAL_TIMER_BASE;

	*(interval_timer_ptr) = 0; 		// clear the interrupt
	if (!paused) {
		(*count_ptr) += 1;			// set global variable
	}

	if (reset) {
		*count_ptr = 0;
		reset = 0;
	}

	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *count_ptr);

	return;
}



