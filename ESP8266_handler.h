#ifndef ESP8266_HANDLER
#define	ESP8266_HANDLER

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>
#include "definitions.h"
#include "uart_handler.h"



//void setRTCCtimeFromAPI(char uart_buffer_ptr[], int sizeOfBuffer);
void sendHttpRequest(char * request, int sizeOfRequest, char * requestResponse);
void handleIncommingMessage();
void initESP8266();
void setRTCCtimeFromServer();
bool connectedToWiFi();
bool connectToSocket();
bool syncAlarmPeriodFromServer();
void hardResetWifiModule();
bool alarmTriggered();
bool registerDevice();
bool getSocketStatus();

#endif	/*ESP8266_HANDLER*/

