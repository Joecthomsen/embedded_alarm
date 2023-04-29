
#ifndef STATE_H
#define	STATE_H

#include <xc.h> 
#include <stdbool.h>
#include "mcc_generated_files/rtcc.h"
#include <time.h>
#include "settings.h"

typedef enum {ACTIVE, NOT_ACTIVE, ALARM, NO_ALARM_PERIOD, NO_WIFI, LOST_CONNECTION, NOT_INITIALIZED, NOT_CONNECTED_TO_DATESERVER, RE_INIT, NOT_CONNECTED_TO_SOCKET} State;

extern int globalState;

void initStatusLED();
void setState(int state);
int getState();
void turnOnRedLED();
void turnOnYellowLED();
void turnOnGreenLED();
void turnOnBlueLED();
void turnOffRedLED();
void turnOffYellowLED();
void turnOffGreenLED();
void turnOffBlueLED();
bool alarmActive();

//enum status getStatus();

#endif	/* STATE_H */

