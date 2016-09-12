/*
 * buttonISR.h
 *
 *  Created on: Apr 18, 2015
 *      Author: brent.horine
 */

#ifndef BUTTONISR_H_
#define BUTTONISR_H_

#include "System.h"
#include "alt_types.h"

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void buttonISR(void* context);
#else
void buttonISR(void* context, alt_u32 id);
#endif

#endif /* BUTTONISR_H_ */
