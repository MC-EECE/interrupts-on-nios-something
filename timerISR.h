/*
 * timerISR.h
 *
 *  Created on: 
 *      Author: 
 */

#ifndef TIMERISR_H_
#define TIMERISR_H_

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void timerISR(void* context);
#else
void timerISR(void* context, alt_u32 id);
#endif

#endif /* TIMERISR_H_ */
