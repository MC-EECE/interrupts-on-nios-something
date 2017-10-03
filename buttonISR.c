/*
 * buttonISR.c
 *
 *  Created on: 
 *      Author: 
 */
#include "System.h"
#include "alt_types.h"
#include "buttonISR.h"
#include "altera_avalon_pio_regs.h"

#define KEY1 2
#define KEY2 4
#define KEY3 8
#define Timer_On 0x7
#define Timer_Off 0xB

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void buttonISR(void* context)
#else
void buttonISR(void* context, alt_u32 id)
#endif
{
	alt_u8 timer_setting;
    /* Cast context It is important that this be declared 
     * volatile to avoid unwanted compiler optimization.
     */
	alt_u32* pCount = (alt_u32*)context;
    /* Store the value in the PUSHBUTTONS's edge capture register in *context. */
	alt_u8 button_pushed = IORD(PUSHBUTTONS_BASE, 3);
    /* Reset the PUSHBUTTONS's edge capture register. */
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0);

    /* Act upon the interrupt */
    switch (button_pushed){

    case KEY1:

    	timer_setting = IORD(INTERVAL_TIMER_BASE, 1);
    	if (timer_setting&0x8){
    		timer_setting = Timer_On;
    		LCD_cursor(0,0);
    		LCD_text("Timer Running ...           ");
    	} else {
    		timer_setting = Timer_Off;
    		LCD_cursor(0,0);
    		LCD_text("Timer Paused ...            ");
    	}
    	IOWR(INTERVAL_TIMER_BASE, 1, timer_setting);
    	break;

    case KEY2:

    	*pCount = 0;
    	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *pCount);
    	LCD_cursor(0,1);
    	LCD_text("Key 2 Pressed ...                ");
    	break;

    case KEY3:

    	toggleBCDHEX();
    	LCD_cursor(0,1);
    	LCD_text("Key 3 Pressed ...                 ");
    	break;

    default:
    	break;

    }



    /*
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}
