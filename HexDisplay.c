/*
 * HexDisplay.c
 *
 *  Created on: Apr 21, 2015
 *      Author: brent.horine
 */
#include "seven_seg_hex.h"
#include "seven_seg_bcd.h"

extern volatile alt_u8 leading_zeros;
extern volatile alt_u8 display_as_bcd;

void HexDisplay(alt_u32 * base, alt_u16 num) {
	/*if (display_as_bcd) {
		seven_seg_bcd(base, num, 0);
	} else {*/
		seven_seg_hex(base, num, 0);
//	}
}
