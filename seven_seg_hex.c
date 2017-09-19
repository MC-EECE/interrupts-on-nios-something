/*
 * seven_seg_hex.c
 *
 *  Created on: Apr 21, 2015
 *      Author: brent.horine
 */


#include "seven_seg_hex.h"
#include "system.h"
#include "io.h"

/*
 * seven_seg_hex - encodes a 16 bit value to display as HEX
 * on a seven segment display.
 * Parameters:
 *     base          - base address of the display
 *     num           - number to display
 *     leading_zeros - if non-zero display leading zeros
 */
void seven_seg_hex(alt_u32 * base, alt_u16 num, alt_u8 leading_zeros) {
	// an array that holds the values for each nibble
	alt_u8 hex[4];
	// encodings for each digit for the 7 LEDs making up the display
	alt_u8 HEX_CODES[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

	hex[0] = num & 0xF;          // least significant digit
	hex[1] = (num >> 4) & 0xF;
	hex[2] = (num >> 8) & 0xF;
	hex[3] = (num >> 12) & 0xF;  // most significant digit

	// encode the digits
	// blank leading zeros if asked, but never other zeros
	alt_u8 trailing_zeros = 0;
	int i;
	int k = 24;
	int hex4 = 0; // The encoded value to write to the base register
	for (i=3; i>=0; i--) {
		if (leading_zeros || hex[i] > 0 || trailing_zeros) {
			hex4 += HEX_CODES[hex[i]] << k;
			trailing_zeros = 1;
		}
		k -= 8;
	}
	IOWR(base, 0, hex4);
}
