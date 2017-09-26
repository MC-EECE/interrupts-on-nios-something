/*
 * buttonISR.c
 *
 *  Created on: 
 *      Author: 
 */

#include "buttonISR.h"
#include "altera_avalon_pio_regs.h"

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void buttonISR(void* context)
#else
void buttonISR(void* context, alt_u32 id)
#endif
{
    /* Cast context It is important that this be declared 
     * volatile to avoid unwanted compiler optimization.
     */
	volatile int * KEY_ptr = (int *)PUSHBUTTONS_BASE;
	volatile int * interval_timer_ptr = (int *)INTERVAL_TIMER_BASE;
	int press;

    /* Store the value in the PUSHBUTTONS's edge capture register in *context. */

    /* Reset the PUSHBUTTONS's edge capture register. */
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0);

    /* Act upon the interrupt */
    press = *(KEY_ptr + 3);
    	*(KEY_ptr + 3) = 0xE;

    	if (press & 0x2)
    	{
    		if (*(interval_timer_ptr + 1) = 0x7)
    		{
    			*(interval_timer_ptr + 1) = 0xB;
    		}

    		else if (*(interval_timer_ptr + 1) = 0xB)
    		{
    			*(interval_timer_ptr + 1) = 0x7;
    		}
    	}

    	else if (press & 0x4)					// KEY2
    		*(interval_timer_ptr + 1) = 0xB;
    	else

    /*
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}
