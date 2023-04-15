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
#include "mcc_generated_files/switch1.h"
#include <string.h>
#include <stdio.h>
#include "mcc_generated_files/delay.h"
#include <stdio.h>
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"
#include <time.h>
#include "mcc_generated_files/rtcc.h"
#include "uart_buffer.h"
#include "state.h"
#include "flashInterface.h"

//void setRTCCtimeFromAPI(char uart_buffer_ptr[], int sizeOfBuffer){
void setRTCCtimeFromAPI(){
    
    
    char * token;
    memset(uart_buffer, '\0', UART_BUFFER_SIZE);      
    
//    uint8_t connect_wifi[] = "AT+CWJAP=\"TheThomsenFamily_2,4GHz\",\"22267892\"\r\n";    
    uint8_t connect_to_time_API[] = "AT+CIPSTART=\"TCP\",\"65.109.143.74\",8080\r\n";
    uint8_t send_size_advicer_to_API[] = "AT+CIPSEND=53\r\n";
    uint8_t send_GET_time[] = "GET /time HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n";    
    
    //Clear buffer.  
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
            setStatus(currentState);
            return;
        }          
    } 
    clearUartBuffer();
    printf("Sending AT size of msg command..\r\n");
    for(size_t i = 0 ; i < sizeof(send_size_advicer_to_API) ; i++){
        UART1_Write(send_size_advicer_to_API[i]);                
    }
    clearUartBuffer();  
    DELAY_milliseconds(100);
    
    printf("Sending GET command 1..\r\n");
    for(size_t i = 0 ; i < sizeof(send_GET_time) ; i++){
        UART1_Write(send_GET_time[i]);                
    }
    clearUartBuffer();
    DELAY_milliseconds(100);
    
    printf("Sending GET command 2..\r\n");
    for(size_t i = 0 ; i < sizeof(send_GET_time) ; i++){
        UART1_Write(send_GET_time[i]);                
    } 

    DELAY_milliseconds(1000);
    
    char dateResponse[512]; 
    for(int i = 0 ; i < sizeof(dateResponse); i++){
        dateResponse[i] = *(uart_buffer+i);
    }
    clearUartBuffer();  

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
    DELAY_milliseconds(500);   //Wait for wifi-module to respond
    
    for(int i = 0 ; i < sizeof(response); i++){
        response[i] = *(uart_buffer+i);
    }
    char * token = strtok(response, ":");    //Make a pointer to the first value ":" buffer and tokenize it. 
    token = strtok(NULL, ":");
    while(token != NULL){
        if(*(token) == '2'){
            return true;
        }
        else{
            token = strtok(NULL, ":");
        }
    }
    return false;
}

void getPeriod(){
    
    //char command[70];
    char command[] = "GET /period/1 HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n";
    char connect_to_API[] = "AT+CIPSTART=\"TCP\",\"65.109.143.74\",8080\r\n";   
    char send_size_advicer_to_API[] = "AT+CIPSEND=59\r\n";
    
    uint16_t rawUserId = getUserId();
    uint8_t arrayToConvert[2];
    arrayToConvert[0]= rawUserId & 0xff;
    arrayToConvert[1]=(rawUserId >> 8);  
    uint8_t userId = arrayToConvert[0];    
    //snprintf(command, sizeof(command), "GET /period/%u HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n", userId);
    
    printf("Sending Connect command ..\r\n");
    for(size_t i = 0 ; i < sizeof(connect_to_API) ; i++){
        UART1_Write(connect_to_API[i]);                
    }
    DELAY_milliseconds(100);
    
    printf("Sending Size command ..\r\n");
    for(size_t i = 0 ; i < sizeof(send_size_advicer_to_API) ; i++){
        UART1_Write(send_size_advicer_to_API[i]);                
    }
    DELAY_milliseconds(100);   
    
    clearUartBuffer();
    
    printf("Sending GET command 1..\r\n");
    for(size_t i = 0 ; i < sizeof(command) ; i++){
        UART1_Write(command[i]);                
    }
    DELAY_milliseconds(100);
    
    printf("Sending GET command 2..\r\n");
    for(size_t i = 0 ; i < sizeof(command) ; i++){
        UART1_Write(command[i]);                
    }
    DELAY_milliseconds(1000);
    
    char response[512] = "\0";
    
    for(int i = 0 ; i < 512; i++){
        response[i] = uart_buffer[i];
    }
    int test = 0;
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
 
