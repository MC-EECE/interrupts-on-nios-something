
#include "io.h"
#include "system.h"
#include "alt_types.h"
#include "timerISR.h"
#include "buttonISR.h"


/*******************************************************************************
 * int main()                                                                  *
 *                                                                             *
 ******************************************************************************/
volatile alt_u8 display_as_bcd;		//Declaration of the variable used to toggle between HEX and BCD, "volatile" has to be declared outside of function
int main(void)
{ 
    /* Setup */

    /* Initialize Variables */
	alt_u32 *pCount;				//Pointer to count variable
	static alt_u32 count = 0;		//Static declaration of count variable so it isn't overwritten
	pCount = &count;				//Assigning the address of count to pCount

	display_as_bcd = 1;				//Assigning 1 as initial value
    /* Register ISRs */

	alt_irq_register(INTERVAL_TIMER_IRQ, (void*) pCount, timerISR);		//Registering timer interrupt service routine
	alt_irq_register(PUSHBUTTONS_IRQ, (void*) pCount, buttonISR);		//Registering button interrupt service routine
	IOWR(PUSHBUTTONS_BASE, 2, 0xE);										//Enabling push button interrupts


    /* Initialize Timer */
	IOWR(INTERVAL_TIMER_BASE, 2, 0xFFFF&INTERVAL_TIMER_FREQ);			//Writing the 16 least sig bits of the frequency to the interval timer counter start value (low)
																		//the interval timer counter start value (low) to increment each second

	IOWR(INTERVAL_TIMER_BASE, 3, 0xFFFF&(INTERVAL_TIMER_FREQ >> 16));	//Writing the 16 most sig bits of the frequency to the interval timer counter start value (low)
																		//the interval timer counter start value (high) to increment each second

	IOWR(INTERVAL_TIMER_BASE, 1, 0x7);									//Enabling the timer by writing 0111 to the control register
    /* Loop while processing interrupts */
    while( 1 ) 
    {

    }
    return 0; // should never get here
}
