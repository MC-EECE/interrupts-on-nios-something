/*
 * buttonISR.c
 *
 *  Created on: Apr 18, 2015
 *      Author: brent.horine
 */

#include "buttonISR.h"
#include "altera_avalon_pio_regs.h"
extern alt_u8 paused;
extern alt_u8 reset;
extern alt_u8 last_key_pressed;

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void buttonISR(void* context)
#else
void buttonISR(void* context, alt_u32 id)
#endif
{
    /* Cast context to edge_capture's type. It is important that this be
     * declared volatile to avoid unwanted compiler optimization.
     */
    volatile int* edge_capture_ptr = (volatile int*) context;
    /* Store the value in the PUSHBUTTONS's edge capture register in *context. */
    *edge_capture_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
    /* Reset the PUSHBUTTONS's edge capture register. */
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0);
    switch (*edge_capture_ptr) {
    case 0x1:
    	// won't get here since KEY0 resets the processor
    	last_key_pressed = 0;
    	break;
    case 0x2:
    	paused = !paused;
    	last_key_pressed = 1;
    	break;
    case 0x4:
    	reset = 1;
    	last_key_pressed = 2;
    	break;
    case 0x8:
    	last_key_pressed = 3;
    	break;
    }
    /*
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}
