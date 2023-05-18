#include <xc.h>
#include "uart_handler.h"
#include "mcc_generated_files/uart1.h"
#include <stdio.h>
#include <string.h>

char uart_buffer[UART_BUFFER_SIZE*2];
//char uart_buffer_copy[UART_BUFFER_SIZE*2];
//int uart_buffer_size = 0;
int uart_buffer_index = 0;   

void uart_interrupt_handler(){
  
    while(UART1_IsRxReady()){                
        uint8_t data = UART1_Read();
        printf("%c", data);
        uart_buffer[uart_buffer_index] = data;
        uart_buffer_index++;
    }
    
            
//    char *ptr_to_result;
//    char needle[] = "setPeriod";
//    ptr_to_result = strstr(uart_buffer, needle);
//    if(ptr_to_result != NULL){
//        int k = 0;
//    }

    
    
    
    
}

void clearUartBuffer(){
    memset(uart_buffer, '\0', sizeof(uart_buffer));
    uart_buffer_index = 0;
    uart_buffer[0] = '\0';
}

