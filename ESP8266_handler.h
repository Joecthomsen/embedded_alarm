#ifndef ESP8266_HANDLER
#define	ESP8266_HANDLER

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>
#include "definitions.h"
#include "uart_buffer.h"



//void setRTCCtimeFromAPI(char uart_buffer_ptr[], int sizeOfBuffer);
void sendHttpRequest(char * request, int sizeOfRequest, char * requestResponse);
void initESP8266();
void setRTCCtimeFromAPI();
bool connected();
bool syncAlarmPeriodFromServer();
void hardResetWifiModule();
bool setAlarm();

#endif	/*ESP8266_HANDLER*/

