
#ifndef STATE_H
#define	STATE_H

#include <xc.h> 

typedef enum {ACTIVE, NOT_ACTIVE, ALARM, NO_ALARM_PERIOD, NO_WIFI, NOT_INITIALIZED, NOT_CONNECTED_TO_DATESERVER, RE_INIT} State;

extern int globalState;

void initStatusLED();
void setStatus(int state);
int getState();

//enum status getStatus();

#endif	/* STATE_H */

