#ifndef ESP8266_HANDLER
#define	ESP8266_HANDLER

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>
#include "definitions.h"

struct alarmPeriod{
    int start;
    int end;
};

void setRTCCtimeFromAPI(char uart_buffer_ptr[], int sizeOfBuffer);
bool connected(char uart_buffer_ptr[], int sizeOfBuffer);
struct alarmPeriod getAlarmPeriod();

#endif	/*ESP8266_HANDLER*/

