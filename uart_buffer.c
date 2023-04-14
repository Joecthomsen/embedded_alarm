#include <xc.h>
#include "uart_buffer.h"
#include "mcc_generated_files/uart1.h"
#include <stdio.h>
#include <string.h>

char uart_buffer[UART_BUFFER_SIZE*2];
//int uart_buffer_size = 0;
int uart_buffer_index = 0;   

void uart_interrupt_handler(){
  
    while(UART1_IsRxReady()){                
        uint8_t data = UART1_Read();
        printf("%c", data);
        uart_buffer[uart_buffer_index] = data;
        uart_buffer_index++;
        int x = 0;
    }
}

void clearUartBuffer(){
    memset(uart_buffer, '\0', sizeof(uart_buffer));
    uart_buffer_index = 0;
    uart_buffer[0] = '\0';
}

