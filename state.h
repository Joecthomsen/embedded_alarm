
#ifndef STATE_H
#define	STATE_H

#include <xc.h> 

typedef enum {ACTIVE, NOT_ACTIVE, ALARM, NO_ALARM_PERIOD, NO_WIFI, NOT_INITIALIZED, NOT_CONNECTED_TO_DATESERVER} State;

extern int state;

void initStatusLED();
void setStatus(int state);

//enum status getStatus();

#endif	/* STATE_H */

