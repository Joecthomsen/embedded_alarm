
#ifndef STATE_H
#define	STATE_H

#include <xc.h> 
#include <stdbool.h>
#include "mcc_generated_files/rtcc.h"
#include <time.h>
#include "settings.h"
#include "LED_handler.h"

typedef enum {ACTIVE, NOT_ACTIVE, ALARM, NO_ALARM_PERIOD, NO_WIFI, CONNECTED_TO_WIFI, CONNECTED_TO_SOCKET, LOST_CONNECTION, NOT_INITIALIZED, RE_INIT, NOT_CONNECTED_TO_SOCKET, NOT_REGISTERED_WITH_SERVER} State;

extern int globalState;

//Help functions
void setState(int state);
int getState();
bool alarmActive();

// State functions
void not_connected_to_socket_state();
void alarmState();
void activeState();
void not_active_state();
void no_wifi_state();
void lost_connection_state();
#endif	/* STATE_H */

