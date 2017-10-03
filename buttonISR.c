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

#define KEY1 2			//For Readability, give meaningful names to numbers used for various settings
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
	alt_u8 timer_setting;	//Variable to be used for toggling clock on or off

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

    case KEY1: //KEY1 is the second bit in the edge capture register, and is represented by 0x2

    	timer_setting = IORD(INTERVAL_TIMER_BASE, 1);	//Assigning current timer state to timer_setting
    	if (timer_setting&0x8){							//Checking if timer is off
    		timer_setting = Timer_On;					//If off, assigns activate value to timer_setting
    		LCD_cursor(0,0);							//Placing cursor on top left block of LCD
    		LCD_text("Timer Running ...           ");	//Displaying message on the LCD
    	} else {										//Checking if timer is on
    		timer_setting = Timer_Off;					//If on, assigns deactivate value to timer_setting
    		LCD_cursor(0,0);							//Placing cursor on top left block of LCD
    		LCD_text("Timer Paused ...            ");	//Displaying message on the LCD
    	}
    	IOWR(INTERVAL_TIMER_BASE, 1, timer_setting);	//Activating or deactivating timer based on previous checks
    	break;

    case KEY2:	//KEY2 is the third bit in the edge capture register, and is represented by 0x4

    	*pCount = 0;									//Resetting counter by setting count to 0
    	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *pCount);	//Displaying count
    	LCD_cursor(0,1);								//Placing cursor on bottom left block of LCD
    	LCD_text("Key 2 Pressed ...                ");	//Displaying message on the LCD
    	break;

    case KEY3:	//KEY3 is the third bit in the edge capture register, and is represented by 0x8

    	toggleBCDHEX();									//Toggling whether the count should be displayed in BCD or HEX
    	LCD_cursor(0,1);								//Placing cursor on bottom left block of LCD
    	LCD_text("Key 3 Pressed ...                 ");	//Displaying message on the LCD
    	break;

    default:											//Default case in case any errors occur
    	break;

    }



    /*
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}
