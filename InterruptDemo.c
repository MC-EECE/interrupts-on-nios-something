/*************************************************************************
 * Copyright (c) 2015 Manhattan College								      *
 * All rights reserved. All use of this software and documentation is     *
 * subject to the License Agreement located at the end of this file below.*
 *************************************************************************/
/******************************************************************************
 *
 * Description
 * ************* 
 * A program that demonstrates the use of interrupts in C by doing the following
 * operations:
 * 	1. Increment count by 1 pps and display on HEX
 * 	2. Display in HEX if SW0 = off, display in BCD if SW0 = on
 * 	3. Counter toggles between run and pause state upon pushing KEY1
 * 	4. Counter resets to zer upon pushing KEY2
 * 	5. Display messages to LCD depending upon which button is pressed:
 * 		a. KEY1: Display current state: "Running" or "Paused"
 * 		b. KEY2: Display "Reset"
 * 		c. KEY3: Display "KEY3 Pressed"
 * Requirements
 * **************
 * This program requires the following devices to be configured:
 *   a Seven Segment Display PIO named 'seven_seg_pio',
 *   an LCD Display named 'lcd_display',
 *   a PUSHBUTTONS PIO named 'PUSHBUTTONS_pio',
 *
 * Peripherals Exercised by SW
 * *****************************
 * Seven Segment Display
 * LCD
 * PUSHBUTTONSs (SW0-SW3)

 * Software Files
 * ****************
 * InterrupDemo.c ==>  This file.
 *                     main() is contained here, as is the lion's share of the
 *                     functionality.
 * InterruptDemo.h ==>  Contains some very simple VT100 ESC sequence defines
 *                     for formatting text to the LCD Display.
 * timerISR.c ==>		Contains the timer interrupt service routine
 *
 * lcd.c ==>			Contains code to write to LCD
 *
 * buttonISR.c ==>		Contains the button interrupt service routine
 *
 * Important Global Variables
 * **************************
 * count - the count to display
 * 
 *
 * Useful Functions
 * *****************
 * count_binary.c (this file) has the following useful functions.
 *   static void sevenseg_set_hex( int hex )
 *     - Defines a hexadecimal display map for the seven segment display.
 *   static void handle_PUSHBUTTONS_interrupts( void* context, alt_u32 id)
 *   static void init_PUSHBUTTONS_pio()
 *     - These are useful functions because they demonstrate how to write
 *       and register an interrupt handler with the system library.
 *
 * count_binary.h 
 *   The file defines some useful VT100 escape sequences for use on the LCD
 *   Display.
 */

#include "InterruptDemo.h"
#include "altera_up_avalon_character_lcd.h"
#include "alt_types.h"
#include "buttonISR.h"
#include "timerISR.h"
#include "lcd.h"
#include "string.h"
alt_u8 leading_zeros;
alt_u8 display_as_bcd;
alt_u8 bcd_hex_cocked;
alt_u8 count_enable;
alt_u8 paused;
alt_u8 reset;
alt_u8 last_key_pressed;

/* A "loop counter" variable. */
static alt_u16 count;
/* A variable to hold the value of the PUSHBUTTONS pio edge capture register. */
volatile int edge_capture;
/* A variable to hold the timer value. */
volatile int timer_capture;

/* Timer functions */
/* 1. Register ISR */


static void init_timer() {
	count = 0;
void * context = (void*)&count;
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
int register_error_code = alt_ic_isr_register(INTERVAL_TIMER_IRQ_INTERRUPT_CONTROLLER_ID, INTERVAL_TIMER_IRQ,
      timerISR, context, 0x0);
#else
    int register_error_code = alt_irq_register( INTERVAL_TIMER_IRQ, context,
      timerISR);
#endif
    if (register_error_code == 0) {
    /* Set timeout for 50 000 000 */
    int counter = INTERVAL_TIMER_FREQ;
    IOWR(INTERVAL_TIMER_BASE, 2, (counter & 0xFFFF));
    IOWR(INTERVAL_TIMER_BASE, 3, ((counter >> 16) & 0xFFFF));

    /* Enable timer interrupt */
    /* STOP = 0, START = 1, CONT = 1, ITO = 1  */
    IOWR(INTERVAL_TIMER_BASE, 1, 0x07);
    }
}

/* Pushbuttons pio functions */

/*
  Some simple functions to:
  1.  Define an interrupt handler function.
  2.  Register this handler in the system.
*/

/*******************************************************************
 * static void handle_pushbuttons_interrupts( void* context, alt_u32 id)*
 *                                                                 *  
 * Handle interrupts from the Pushbuttons.                             *
 * This interrupt event is triggered by a Pushbutton/switch press.     *
 * This handler sets *context to the value read from the PUSHBUTTONS    *
 * edge capture register.  The PUSHBUTTONS edge capture register        *
 * is then cleared and normal program execution resumes.           *
 * The value stored in *context is used to control program flow    *
 * in the rest of this program's routines.                         *
 *                                                                 *
 * Provision is made here for systems that might have either the   *
 * legacy or enhanced interrupt API active, or for the Nios II IDE *
 * which does not support enhanced interrupts. For systems created *
 * using the Nios II softawre build tools, the enhanced API is     *
 * recommended for new designs.                                    *
 ******************************************************************/
#ifdef PUSHBUTTONS_BASE

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void handle_pushbutton_interrupts(void* context)
#else
static void handle_pushbutton_interrupts(void* context, alt_u32 id)
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
    
    /* 
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}

/* Initialize the PUSHBUTTONS_pio. */

static void init_pushbuttons_pio()
{
    /* Recast the edge_capture pointer to match the alt_irq_register() function
     * prototype. */
    void* edge_capture_ptr = (void*) &edge_capture;
    /* Enable all 4 PUSHBUTTONS interrupts. */
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PUSHBUTTONS_BASE, 0xf);
    /* Reset the edge capture register. */
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0x0);
    /* Register the interrupt handler. */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
    alt_ic_isr_register(PUSHBUTTONS_PIO_IRQ_INTERRUPT_CONTROLLER_ID, PUSHBUTTONS_PIO_IRQ,
    	buttonISR, edge_capture_ptr, 0x0);
     // handle_pushbutton_interrupts, edge_capture_ptr, 0x0);
#else
    alt_irq_register( PUSHBUTTONS_IRQ, edge_capture_ptr,
    	buttonISR);
      //handle_pushbutton_interrupts);
#endif
    IOWR(PUSHBUTTONS_BASE, 2, 0xf);
}
#endif /* PUSHBUTTONS_PIO_BASE */

/* Seven Segment Display PIO Functions 
 * sevenseg_set_hex() --  implements a hex digit map.
 */
 
#ifdef HEX3_HEX0_BASE
static void sevenseg_set_hex(int hex)
{
    static alt_u8 segments[16] = {
        0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84, /* 0-9 */
        0x88, 0xE0, 0xF2, 0xC2, 0xB0, 0xB8 };                       /* a-f */

    unsigned int data = segments[hex & 15] | (segments[(hex >> 4) & 15] << 8);

    IOWR_ALTERA_AVALON_PIO_DATA(HEX3_HEX0_BASE, data);
}
#endif

/* Functions used in main loop
 * lcd_init() -- Writes a simple message to the top line of the LCD.
 * initial_message() -- Writes a message to stdout (usually JTAG_UART).
 * count_<device>() -- Implements the counting on the respective device.
 * handle_PUSHBUTTONS_press() -- Determines what to do when one of the PUSHBUTTONSs
 * is pressed.
 */
static void lcd_init( alt_up_character_lcd_dev *lcd )
{
//	char str[33];
    /* If the LCD Display exists, write a simple message on the first line. */
   // sprintf(str, "%c%s Counting will be displayed below...", ESC,
       //        ESC_TOP_LEFT);
    //sprintf(str, "Counting will be displayed below...");
  	alt_up_character_lcd_string(lcd, "Counting will be displayed below...");

}

static void display(alt_up_character_lcd_dev *lcd_dev) {
	char str[17];
	switch (last_key_pressed) {
	case 0:
		strcpy(str, "..KEY0 pressed..");
		break;
	case 1:
		if (paused) {
			strcpy(str, ".....paused.....");
		} else {
			strcpy(str, "....running.....");
		}
		break;
	case 2:
		strcpy(str, ".....RESET......");
		break;
	case 3:
		strcpy(str, "..KEY3 pressed..");
		break;
	}
	alt_up_character_lcd_set_cursor_pos(lcd_dev, 0, 1);
	alt_up_character_lcd_string(lcd_dev, str);
}

static void initial_message(alt_up_character_lcd_dev *lcd_dev)
{
	alt_up_character_lcd_string(lcd_dev, "Counting Begun");
}

/********************************************************
 * The following functions write the value of the global*
 * variable 'count' to 3 peripherals, if they exist in  *
 * the system.  Specifically:                           *
 * The LEDs will illuminate, the Seven Segment Display  *
 * will count from 00-ff, and the LCD will display the  *
 * hex value as the program loops.                      *
 * *****************************************************/

/* static void count_led()
 * 
 * Illuminate LEDs with the value of 'count', if they
 * exist in the system
 */

static void count_led()
{
#ifdef LED_PIO_BASE
    IOWR_ALTERA_AVALON_PIO_DATA(
        LED_PIO_BASE,
        count
        );
#endif
}

/* static void count_sevenseg()
 * 
 * Display value of 'count' on the Seven Segment Display
 */

static void count_sevenseg()
{
#ifdef HEX3_HEX0_BASE
    sevenseg_set_hex(count);
#endif
}

void pollSW() {
	alt_u16 switches;
	switches = *((alt_u16*)SLIDER_SWITCHES_BASE);

	if ((switches & 0x01) && bcd_hex_cocked) {
		display_as_bcd = !display_as_bcd;
		bcd_hex_cocked = 0;
	}
	bcd_hex_cocked = !(switches & 0x01);
	leading_zeros = (switches & 0x02) >> 1;
}
    
/*******************************************************************************
 * int main()                                                                  *
 *                                                                             *
 * Implements a continuous loop counting from 00 to FF.  'count' is the loop   *
 * counter.                                                                    *
 * The value of 'count' will be displayed on one or more of the following 3    *
 * devices, based upon hardware availability:  LEDs, Seven Segment Display,    *
 * and the LCD Display.                                                        *
 *                                                                             *
 *                                                             *
 ******************************************************************************/

int main(void)
{ 
    alt_up_character_lcd_dev * lcd;

    count = 0;

    /* Initialize the LCD, if there is one.     */
    lcd = alt_up_character_lcd_open_dev(CHAR_LCD_16X2_NAME);
    if(lcd != NULL) {alt_up_character_lcd_init( lcd );}
    
    /* Initialize the PUSHBUTTONS pio. */

#ifdef PUSHBUTTONS_BASE
    init_pushbuttons_pio();
#endif

    /* Initialize the timer */
#ifdef INTERVAL_TIMER_BASE
    init_timer();
#endif

/* Initial message to output. */

    initial_message(lcd);

/* Enable global interrupts */

    NIOS2_WRITE_IENABLE( 0x3 );	// set interrupt mask bits for levels 0 (interval
    							// timer), 1 (pushbuttons), 6 (audio), and 7 (PS/2)

    NIOS2_WRITE_STATUS( 1 );		// enable Nios II interrupts

    count_enable = 1;

    while( 1 ) 
    {
        pollSW();
        display(lcd);
    }
    LCD_CLOSE(lcd);
    return 0;
}
/******************************************************************************
 *                                                                             *
 * License Agreement                                                           *
 *                                                                             *
 * Copyright (c) 2006 Altera Corporation, San Jose, California, USA.           *
 * All rights reserved.                                                        *
 *                                                                             *
 * Permission is hereby granted, free of charge, to any person obtaining a     *
 * copy of this software and associated documentation files (the "Software"),  *
 * to deal in the Software without restriction, including without limitation   *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
 * and/or sell copies of the Software, and to permit persons to whom the       *
 * Software is furnished to do so, subject to the following conditions:        *
 *                                                                             *
 * The above copyright notice and this permission notice shall be included in  *
 * all copies or substantial portions of the Software.                         *
 *                                                                             *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
 * DEALINGS IN THE SOFTWARE.                                                   *
 *                                                                             *
 * This agreement shall be governed in all respects by the laws of the State   *
 * of California and by the laws of the United States of America.              *
 * Altera does not recommend, suggest or require that this reference design    *
 * file be used in conjunction or combination with any other product.          *
 ******************************************************************************/
