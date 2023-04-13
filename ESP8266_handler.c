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

void setRTCCtimeFromAPI(char uart_buffer_ptr[], int sizeOfBuffer){
    
    char cpy[1024*2+1];     
    memset(uart_buffer_ptr, '\0', sizeOfBuffer);      
    memset(cpy, '\0', sizeof(cpy));
    
//    uint8_t connect_wifi[] = "AT+CWJAP=\"TheThomsenFamily_2,4GHz\",\"22267892\"\r\n";    
    uint8_t connect_to_time_API[] = "AT+CIPSTART=\"TCP\",\"65.109.143.74\",8080\r\n";
    uint8_t send_size_advicer_to_API[] = "AT+CIPSEND=53\r\n";
    uint8_t send_GET_time[] = "GET /time HTTP/1.0\r\nHost: 65.109.143.74\r\n\r\n\r\n";    
    
    printf("Sending AT connect to API command..\r\n");
    for(size_t i = 0 ; i < sizeof(connect_to_time_API) ; i++){
        UART1_Write(connect_to_time_API[i]);                
    }
    
    DELAY_milliseconds(100);

    
    printf("Sending AT size of msg command..\r\n");
    for(size_t i = 0 ; i < sizeof(send_size_advicer_to_API) ; i++){
        UART1_Write(send_size_advicer_to_API[i]);                
    }
    
    DELAY_milliseconds(100);
    
    printf("Sending GET command 1..\r\n");
    for(size_t i = 0 ; i < sizeof(send_GET_time) ; i++){
        UART1_Write(send_GET_time[i]);                
    }
    
    DELAY_milliseconds(100);
    
    printf("Sending GET command 2..\r\n");
    for(size_t i = 0 ; i < sizeof(send_GET_time) ; i++){
        UART1_Write(send_GET_time[i]);                
    } 
    
    DELAY_milliseconds(1000);
    for(int i = 0 ; i < sizeOfBuffer; i++){
        cpy[i] = *(uart_buffer_ptr+i);
    }
    //strcpy(cpy, uart_buffer_ptr);

//TODO check if received package is valid. 
            
            char * token = strtok(cpy, "\"");    //Make a pointer to the buffer and tokenize it. 
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            printf("\r\n\n");  
//            char fullDate[40];
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
            int testw = 0;
}

bool connected(char uart_buffer_ptr[], int sizeOfBuffer){
    uint8_t AT_status_cmd[] = "AT+CIPSTATUS\r\n";
    for(int i = 0 ; i < sizeof(AT_status_cmd) ; i++){
        UART1_Write(AT_status_cmd[i]);
    }
    
    char cpy[1024*2+1];     
    memset(uart_buffer_ptr, '\0', sizeOfBuffer);      
    memset(cpy, '\0', sizeof(cpy));
    DELAY_milliseconds(100);
    
    for(int i = 0 ; i < sizeOfBuffer; i++){
        cpy[i] = *(uart_buffer_ptr+i);
    }
    
    char * token = strtok(cpy, ":");    //Make a pointer to the buffer and tokenize it. 
    token = strtok(NULL, ":");
    
    int test = 0;
    return true;
}

