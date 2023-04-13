
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> 

enum status {ACTIVE, NOT_ACTIVE, ALARM, NO_ALARM_PERIOD, NO_WIFI};

void initStatusLED();
void setStatus(int status);
//enum status getStatus();

#endif	/* XC_HEADER_TEMPLATE_H */

