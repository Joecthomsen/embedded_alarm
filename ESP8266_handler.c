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
#include "uart_buffer.h"
#include "stateManager.h"
#include "flashInterface.h"
#include "settings.h"

void sendHttpRequest(char * request, int sizeOfRequest, char * requestResponse){
    
    char send_size_to_API[24];
    memset(send_size_to_API,'\0', sizeof(send_size_to_API));    
    sprintf(send_size_to_API, "AT+CIPSEND=%d\r\n", sizeOfRequest);
    char * token;
    memset(uart_buffer, '\0', UART_BUFFER_SIZE);       
    uint8_t connect_to_time_API[] = "AT+CIPSTART=\"TCP\",\"65.109.143.74\",8080\r\n";

// CONNECT TO API
    clearUartBuffer();
    printf("Sending AT connect to API command..\r\n");
    for(size_t i = 0 ; i < sizeof(connect_to_time_API) ; i++){
        UART1_Write(connect_to_time_API[i]);                
    }
    
    DELAY_milliseconds(100);    
    char firstResponse[128] = "\0";
    
    for(int i = 0 ; i < 128; i++){
        firstResponse[i] = uart_buffer[i];
    }
    
    token = strtok(firstResponse, "\n");
    token = strtok(NULL, "\n");    
    char stringConnect[7] = "CONNECT";
    char stringAlready[7] = "ALREADY";
    char readStringFromBuffer[sizeof(stringConnect)];
    for(int i = 0 ; i < sizeof(stringConnect) ; i++){
        readStringFromBuffer[i] = *(token+i);
    }
    int compaireConnect = strcmp(stringConnect, stringConnect);
    int compaireAlready = strcmp(stringConnect, stringAlready);
    if(!compaireConnect){
        if(!compaireAlready){
            State currentState = NOT_CONNECTED_TO_DATESERVER;
            setState(currentState);
            return;
        }          
    }

// Send size advice of GET/POST/PUT request
    clearUartBuffer();
    printf("Sending AT size of msg command..\r\n");
    for(size_t i = 0 ; send_size_to_API[i] != '\0' ; i++){
        UART1_Write(send_size_to_API[i]);                
    }
    clearUartBuffer();  
    DELAY_milliseconds(100);

//Send first http request 
    
    printf("Sending GET command 1..\r\n");
    for(size_t i = 0 ; i < sizeOfRequest ; i++){
        UART1_Write(*(request+i));                
    }
    //clearUartBuffer();
    char response[512];
    memset(response, '\0', sizeof(response));
    DELAY_milliseconds(100);
    for(int i = 0 ; *(uart_buffer+i) != '\0' ; i++){
        *(requestResponse+i) = *(uart_buffer+i);
        response[i] = *(uart_buffer+i);
    }
    clearUartBuffer();     
}

void setRTCCtimeFromAPI(){

    char * token;
    char dateResponse[512];
    memset(dateResponse, '\0', sizeof(dateResponse));
    char send_GET_time[] = "GET /time HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n"; 
        //snprintf(command, sizeof(command), "GET /period/%u HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n", userId);
    sendHttpRequest(send_GET_time, 53, dateResponse);

    //TODO check if received package is valid. 
            
    token = strtok(dateResponse, "\"");    //Make a pointer to the buffer and tokenize it. 
    token = strtok(NULL, "\"");
    printf("\r\n\n");  
    char year[4];
    char month[2];
    char date[2];
    char hour[2];
    char minute[2];
    char second[2];

    for(int i = 0 ; i < 4 ; i++){
        year[i] = *(token+i);
    }
    for(int i = 0 ; i < 2 ; i++){
        month[i] = *(token+5+i);
    }
    for(int i = 0 ; i < 2 ; i++){
        date[i] = *(token+8+i);
    }
    for(int i = 0 ; i < 2 ; i++){
        hour[i] = *(token+11+i);
    }
    for(int i = 0 ; i < 2 ; i++){
        minute[i] = *(token+14+i);
    }
    for(int i = 0 ; i < 2 ; i++){
        second[i] = *(token+17+i);
    }

    struct tm time;

    time.tm_year = (1000*(year[0] - 48 ) + 100*(year[1] - 48) + 10*(year[2] - 48) + (year[3] - 48)) - 1900;
    time.tm_mon = 10*(month[0] - 48) + month[1] - 48;
    time.tm_mday = 10*(date[0] - 48) + date[1] - 48;
    time.tm_hour = 10*(hour[0] - 48) + hour[1] - 48 + 2; //TODO Enable timezone managment
    time.tm_min = 10*(minute[0] - 48) + minute[1] - 48;
    time.tm_sec = 10*(second[0] - 48) + second[1] - 48;
    time.tm_wday=1;
    time.tm_yday=1;
    RTCC_TimeSet(&time); 
    
    /*
    struct tm getTimeStruct;
    RTCC_TimeGet(&getTimeStruct);
    */
}

bool connected(){
    
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
    char * token = strtok(response, ":");    //Make a pointer to the first value ":" buffer and tokenize it. 
    token = strtok(NULL, ":"); 
    while(token != NULL){
        if(*(token) == '5'){
            return false;
        }
        else{
            token = strtok(NULL, ":");
        }
    }
    return true;
}

bool syncAlarmPeriodFromServer(){
    
    int sizeOfRequest = 59;  
    char response[512];
    char request[sizeOfRequest];
       
    snprintf(request, sizeof(request), "GET /period/%d HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n", getUserId());  //Build the request with unique device ID
    sendHttpRequest(request, sizeOfRequest, response);
        
    char needle[] = "1 200";
    char *ptr_to_200_OK;
    ptr_to_200_OK = strstr(response, needle);
    
    if(ptr_to_200_OK != NULL){  //If everything is okay, we need to destructure the json object containing the values.
        char startTime[5];
        char endTime[5];
        char * ptr_to_value = strstr(response, "startTime");
        ptr_to_value = strtok(ptr_to_value, ":");
        ptr_to_value = strtok(NULL, ":");
        for(size_t i = 0 ; *(ptr_to_value+i+1) != '\"' ; i++){
            startTime[i] = *(ptr_to_value+i+1);
        }
        ptr_to_value = strtok(NULL, ":");
        for(size_t i = 0 ; *(ptr_to_value+i+1) != '\"' ; i++){
            endTime[i] = *(ptr_to_value+i+1);
        }
        
        int startTimeConverted;
        int endTimeConverted;

        startTimeConverted = 1000*(startTime[0] - 48) + 100*(startTime[1] - 48) + 10*(startTime[2] - 48) + (startTime[3] - 48);     
        endTimeConverted = 1000*(endTime[0] - 48) + 100*(endTime[1] - 48) + 10*(endTime[2] - 48) + (endTime[3] - 48);        
        setAlarmPeriod(startTimeConverted, endTimeConverted);   
        return true;
    }   
    return false;
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
 
bool setAlarm(){
    
    int sizeOfRequest = 59;
    char request[sizeOfRequest];      
    char response[512];// = "\0";
    
    snprintf(request, sizeof(request), "POST /alarm/%d HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n", getUserId());
    sendHttpRequest(request, sizeOfRequest, response);

    char needle[] = "1 200";
    char *ptr_to_200_OK;
    ptr_to_200_OK = strstr(response, needle);
    if(ptr_to_200_OK != NULL){
        return true;
    }
    return false;
}   
