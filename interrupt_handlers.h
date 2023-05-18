
#ifndef TIMER_INTERRUPT_H
#define	TIMER_INTERRUPT_H

#include <xc.h> 
#include "ESP8266_handler.h"

void timer1_interrupt_handler();
//void timer2_interrupt_handler();
void timer3_interrupt_handler();
void RTCC_InterrurpHandler();
#endif	/* TIMER_INTERRUPT_H */

