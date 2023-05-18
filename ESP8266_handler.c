/*
 * File:   ESP8266_handler.c
 * Author: Joect
 *
 * Created on April 10, 2023, 8:43 PM
 */

#include <xc.h>
#include "ESP8266_handler.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/uart1.h"
#include <string.h>
#include <stdio.h>
#include "mcc_generated_files/delay.h"
#include <stdio.h>
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"
#include <time.h>
#include "mcc_generated_files/rtcc.h"
#include "uart_handler.h"
#include "stateManager.h"
#include "flashInterface.h"
#include "settings.h"

//void sendHttpRequest(char * request, int sizeOfRequest, char * requestResponse){
//    
//    char send_size_to_API[24];
//    memset(send_size_to_API,'\0', sizeof(send_size_to_API));    
//    sprintf(send_size_to_API, "AT+CIPSEND=%d\r\n", sizeOfRequest);
//    char * token;
//    memset(uart_buffer, '\0', UART_BUFFER_SIZE);       
//    uint8_t connect_to_time_API[] = "AT+CIPSTART=\"TCP\",\"65.109.143.74\",8080\r\n";
//
//// CONNECT TO API
//    clearUartBuffer();
//    printf("Sending AT connect to API command..\r\n");
//    for(size_t i = 0 ; i < sizeof(connect_to_time_API) ; i++){
//        UART1_Write(connect_to_time_API[i]);                
//    }
//    
//    DELAY_milliseconds(100);    
//    char firstResponse[128] = "\0";
//    
//    for(int i = 0 ; i < 128; i++){
//        firstResponse[i] = uart_buffer[i];
//    }
//    
//    token = strtok(firstResponse, "\n");
//    token = strtok(NULL, "\n");    
//    char stringConnect[7] = "CONNECT";
//    char stringAlready[7] = "ALREADY";
//    char readStringFromBuffer[sizeof(stringConnect)];
//    for(int i = 0 ; i < sizeof(stringConnect) ; i++){
//        readStringFromBuffer[i] = *(token+i);
//    }
//    int compaireConnect = strcmp(stringConnect, stringConnect);
//    int compaireAlready = strcmp(stringConnect, stringAlready);
//    if(!compaireConnect){
//        if(!compaireAlready){
//            State currentState = NOT_CONNECTED_TO_DATESERVER;
//            setState(currentState);
//            return;
//        }          
//    }
//
//// Send size advice of GET/POST/PUT request
//    clearUartBuffer();
//    printf("Sending AT size of msg command..\r\n");
//    for(size_t i = 0 ; send_size_to_API[i] != '\0' ; i++){
//        UART1_Write(send_size_to_API[i]);                
//    }
//    clearUartBuffer();  
//    DELAY_milliseconds(100);
//
////Send first http request 
//    
//    printf("Sending GET command 1..\r\n");
//    for(size_t i = 0 ; i < sizeOfRequest ; i++){
//        UART1_Write(*(request+i));                
//    }
//    //clearUartBuffer();
//    char response[512];
//    memset(response, '\0', sizeof(response));
//    DELAY_milliseconds(100);
//    for(int i = 0 ; *(uart_buffer+i) != '\0' ; i++){
//        *(requestResponse+i) = *(uart_buffer+i);
//        response[i] = *(uart_buffer+i);
//    }
//    clearUartBuffer();     
//}

void setRTCCtimeFromServer(){

    char dateResponse[512];
    memset(dateResponse, '\0', sizeof(dateResponse));
    uint8_t send_GET_time[] = "{\"request\":\"getDate\"}\r\n"; 
    //snprintf(command, sizeof(command), "GET /period/%u HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n", userId);
    //sendHttpRequest(send_GET_time, 53, dateResponse);
    
    clearUartBuffer();
    while(!UART1_IsTxReady()){;}
    printf("Sending time request to server..\r\n");
    for(size_t i = 0 ; i < sizeof(send_GET_time) ; i++){
        UART1_Write(send_GET_time[i]);
    }
    char response[56];  
    DELAY_milliseconds(500);   //Wait for wifi-module to respond
    
    for(size_t i = 0 ; i < sizeof(response); i++){
        response[i] = *(uart_buffer+i);
    }
    
    //TODO check if received package is valid. 
            
    char year[4];
    char month[2];
    char date[2];
    char hour[2];
    char minute[2];
    char second[2];
    
    for(int i = 0 ; i < 2 ; i++){
        date[i] = response[i+1];
    }
    for(int i = 0 ; i < 2 ; i++){
        month[i] = response[i+4];
    }
    for(int i = 0 ; i < 4 ; i++){
        year[i] = response[i+7];
    } 
    
    for(int i = 0 ; i < 2 ; i++){
        hour[i] = response[i+12];
    }
    for(int i = 0 ; i < 2 ; i++){
        minute[i] = response[i+15];
    }
    for(int i = 0 ; i < 2 ; i++){
        second[i] = response[i+18];
    }

    struct tm time;

    time.tm_year = (1000*(year[0] - 48 ) + 100*(year[1] - 48) + 10*(year[2] - 48) + (year[3] - 48)) - 1900;
    time.tm_mon = 10*(month[0] - 48) + month[1] - 48;
    time.tm_mday = 10*(date[0] - 48) + date[1] - 48;
    time.tm_hour = 10*(hour[0] - 48) + hour[1] - 48; //TODO Enable timezone managment
    time.tm_min = 10*(minute[0] - 48) + minute[1] - 48;
    time.tm_sec = 10*(second[0] - 48) + second[1] - 48;
    time.tm_wday=1;
    time.tm_yday=1;
    RTCC_TimeSet(&time);    
    
    struct tm getTimeStruct;
    RTCC_TimeGet(&getTimeStruct);
    int k = 0;
    clearUartBuffer();
    
}

bool connectedToWiFi(){
    
    clearUartBuffer();
    uint8_t AT_status_cmd[] = "AT+CIPSTATUS\r\n";
    if(UART1_IsTxReady()){
        for(int i = 0 ; i < sizeof(AT_status_cmd) ; i++){
            UART1_Write(AT_status_cmd[i]);
        }
    }
    
    char response[256];  
    DELAY_milliseconds(50);   //Wait for wifi-module to respond
    
    for(int i = 0 ; i < sizeof(response); i++){
        response[i] = *(uart_buffer+i);
    }
    int k = 0;
    char * token = strtok(response, ":");    //Make a pointer to the first value ":" buffer and tokenize it. 
    token = strtok(NULL, ":");
    
    
    while(token != NULL){
        if(*(token) == '4'  || *(token) == '3'  || *(token) == '2' || *(token) == '1'){
            clearUartBuffer();
//                if(getState() == CONNECTED_TO_SOCKET || getState() == ACTIVE || getState() == NOT_ACTIVE){
//                    for(size_t i = 0 ; i < sizeof(cipMode1) ; i++){
//                        UART1_Write(cipMode1[i]);
//                    }
//                    for(size_t i = 0 ; i < sizeof(cipSend) ; i++){
//                        UART1_Write(cipSend[i]);
//                    }                 
//                }     
            int i = 0;
            return true;
        }
        else{
            token = strtok(NULL, ":");}
    }
    int test = 0;
    clearUartBuffer();
    return false;;
}

void hardResetWifiModule(){
    _LATB5 = 0;
    DELAY_milliseconds(25);
    _LATB5 = 1;
    DELAY_milliseconds(2000);
}

void initESP8266(){
    _TRISB5 = 0;
    _LATB5 = 1;
}
 
bool alarmTriggered(){
    
    char alarmRequest[] = "{\"request\":\"alarm\",\"owner\":\"4321\"}\r\n";
    for(size_t i = 0 ; i < sizeof(alarmRequest) ; i++){
        UART1_Write(alarmRequest[i]);
    }
    int k = 0;
    
    return true;
    
    // snprintf(request, sizeof(request), "POST /alarm/%d HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n", getUserId());
    //sendHttpRequest(request, sizeOfRequest, response);

   // char needle[] = "1 200";
   // char *ptr_to_200_OK;
//    ptr_to_200_OK = strstr(response, needle);
//    if(ptr_to_200_OK != NULL){
//        return true;
//    }
//    return false;
}   

bool connectToSocket(){
    
    char connectToSocketCmd[] = "AT+CIPSTART=\"TCP\",\"65.109.143.74\",9090\r\n";
    //char connectToSocketCmd[] = "AT+CIPSTART=\"TCP\",\"127.0.0.1\",9090\r\n";
    char cipModeCmd[] = "AT+CIPMODE=1\r\n";
    char atCipSend[] = "AT+CIPSEND\r\n";
    memset(uart_buffer, '\0', UART_BUFFER_SIZE);  
    
    // CONNECT TO SOCKET
    clearUartBuffer();
    printf("Sending AT connect to socket command..\r\n");
    for(size_t i = 0 ; i < sizeof(connectToSocketCmd) ; i++){
        UART1_Write(connectToSocketCmd[i]);                
    }
    
    DELAY_milliseconds(100);    
    char connectResponse[128] = "\0";
    
    for(int i = 0 ; i < 128; i++){
        connectResponse[i] = uart_buffer[i];
    }
    char *ptr_to_OK;
    char needle[] = "OK";
    ptr_to_OK = strstr(connectResponse, needle);
    if(ptr_to_OK == NULL){
        return false;
    }
    
    //Set transfer mode to transparent transmission
    clearUartBuffer();
    printf("Sending transfer mode command to socket..\r\n");
    for(size_t i = 0 ; i < sizeof(cipModeCmd) ; i++){
        UART1_Write(cipModeCmd[i]);                
    }
    
    DELAY_milliseconds(100);    
    
    for(int i = 0 ; i < sizeof(atCipSend); i++){
        UART1_Write(atCipSend[i]);
    }
    clearUartBuffer();
    return true; 
}

bool registerDevice(){
    
    memset(uart_buffer, '\0', UART_BUFFER_SIZE);  
   //char request[159]; 
   // snprintf(request, sizeof(request), "{\"request\":\"register\",\"type\":\"node\",\"deviceId\":\"%d\",\"owner\":\"4321\",\"nicName\":\"stuen\",\"startTime\":\"2000\",\"endTime\":\"0700\"}\r\n", getUserId());  //Build the request with unique device ID
    char registerRequest[] = "{\"request\":\"register\",\"type\":\"node\",\"deviceId\":\"1\",\"owner\":\"4321\",\"nicName\":\"stuen\",\"startTime\":\"2000\",\"endTime\":\"0700\"}\r\n";
    // Send register command
    clearUartBuffer();
    printf("Sending AT connect to API command..\r\n");
    
    while(!UART1_IsTxReady()){;}
    for(size_t i = 0 ; i < sizeof(registerRequest) ; i++){
        UART1_Write(registerRequest[i]);                
    }
    
    DELAY_milliseconds(500);    
    char registerResponse[256] = "\0";
    
    for(int i = 0 ; i < sizeof(registerResponse); i++){
        registerResponse[i] = uart_buffer[i];
    }
    
    char *ptr_to_OK;
    char needle[] = "Device node registered: OK";
    ptr_to_OK = strstr(registerResponse, needle);
    if(ptr_to_OK == NULL){
        clearUartBuffer();
        return false;
    }
    clearUartBuffer();
    return true;
}

bool getSocketStatus(){
    
    char plus[] = "+++";
    char cipMode0[] = "AT+CIPMODE=0\r\n";
    char cipMode1[] = "AT+CIPMODE=1\r\n";
    char cipSend[] = "AT+CIPSEND\r\n";
    char cipStatus[] = "AT+CIPSTATUS\r\n";
    
    int state = getState();
    
    if(state == CONNECTED_TO_SOCKET || state == ACTIVE || state== NOT_ACTIVE){     
        for(size_t i = 0 ; i < sizeof(plus) - 1 ; i++){
            UART1_Write(plus[i]);
        }
        DELAY_milliseconds(500); 
        for(size_t i = 0 ; i < sizeof(cipMode0) ; i++){
            UART1_Write(cipMode0[i]);
        }
        DELAY_milliseconds(50); 
        for(size_t i = 0 ; i < sizeof(cipStatus) ; i++){
            UART1_Write(cipStatus[i]);
        }  
    }
    char response[256];  
    DELAY_milliseconds(50);   //Wait for wifi-module to respond
    
    for(int i = 0 ; i < sizeof(response); i++){
        response[i] = *(uart_buffer+i);
    }
    int k = 0;
    char * token = strtok(response, ":");    //Make a pointer to the first value ":" buffer and tokenize it. 
    token = strtok(NULL, ":");
    
    
    while(token != NULL){
        if(*(token) == '2' || *(token) == '1'){
            clearUartBuffer();
            for(size_t i = 0 ; i < sizeof(cipMode1) ; i++){
                UART1_Write(cipMode1[i]);
            }
            DELAY_milliseconds(25);
            for(size_t i = 0 ; i < sizeof(cipSend) ; i++){
                UART1_Write(cipSend[i]);
            }
            return true;
        }
        else{
            token = strtok(NULL, ":");}
    }
    int test = 0;
    clearUartBuffer();
    return false;;
}

void handleIncommingMessage(){
        char message[256];
        DELAY_milliseconds(1000);   //Give time for the UART message to arrive
        for(size_t i = 0 ; i < sizeof(message) ; i++){
            message[i] = uart_buffer[i];
        }
        char *ptr_to_search_result;
        char needle[] = "setPeriod";
        ptr_to_search_result = strstr(uart_buffer, needle);
        if(ptr_to_search_result != NULL){
            //get new startTime
            char needleStart[] = "startTime";    //12 fremme er tiden
            char needleEnd[] = "endTime";    //10 fremme er tiden
            ptr_to_search_result = strstr(message, needleStart);
            int startTime = (*(ptr_to_search_result+12)-48)*1000 + (*(ptr_to_search_result+13)-48)*100 + (*(ptr_to_search_result + 14)-48)*10 + (*(ptr_to_search_result+15)-48);
            //get new endTime
            ptr_to_search_result = strstr(message, needleEnd);
            int endTime = (*(ptr_to_search_result+10)-48)*1000 + (*(ptr_to_search_result+11)-48)*100 + (*(ptr_to_search_result + 12)-48)*10 + (*(ptr_to_search_result+13)-48);
            setAlarmPeriod(startTime, endTime);
            if(alarmActive()){
                State state = ACTIVE;
                setState(state);
            }
            else{
                State state = NOT_ACTIVE;
                setState(state);
            }
            memset(message, '\0', sizeof(message));
            clearUartBuffer();
        }
}