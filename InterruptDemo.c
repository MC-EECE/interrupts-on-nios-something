
#include "io.h"
#include "system.h"
#include "alt_types.h"
#include "timerISR.h"

/*******************************************************************************
 * int main()                                                                  *
 *                                                                             *
 ******************************************************************************/

int main(void)
{ 
    /* Setup */
	volatile int *interval_timer_ptr = (int*) INTERVAL_TIMER_BASE;
	volatile int *KEY_ptr = (int*) PUSHBUTTONS_BASE;
	static int counter = 0x2FAF080;//Counter Working, 50000000

    /* Initialize Variables */
	alt_u32 *pCount;
	static alt_u32 count = 0;
	pCount = &count;

    /* Register ISRs */

	alt_irq_register(INTERVAL_TIMER_IRQ, (void*) pCount, timerISR);
	IOWR(PUSHBUTTONS_BASE, 2, 0xE);


    /* Initialize Timer */
	IOWR(INTERVAL_TIMER_BASE, 2, 0xFFFF&INTERVAL_TIMER_FREQ);
	IOWR(INTERVAL_TIMER_BASE, 3, 0xFFFF&(INTERVAL_TIMER_FREQ >> 16));
	IOWR(INTERVAL_TIMER_BASE, 1, 0x7);
    /* Loop while processing interrupts */
    while( 1 ) 
    {

    }
    return 0; // should never get here
}
