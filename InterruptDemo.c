
#include "io.h"
#include "system.h"
#include "alt_types.h"

/*******************************************************************************
 * int main()                                                                  *
 *                                                                             *
 ******************************************************************************/

int main(void)
{ 
    /* Setup */
	static int counter = 0x2FAF080;
	alt_u32 *pCount;
	alt_u32 count = 0;
	pCount=&count;

    /* Initialize Variables */
	volatile int * interval_timer_ptr = (int *) INTERVAL_TIMER_BASE;		// interal timer base address

    /* Register ISRs */

	alt_irq_register(INTERVAL_TIMER_IRQ, (void*) pCount, timerISR);
	IOWR(PUSHBUTTONS_BASE, 2, 0xE);

    /* Initialize Timer */
		*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
		*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;

    /* Loop while processing interrupts */
    while( 1 ) 
    {

    }
    return 0; // should never get here
}
