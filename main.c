
#include "mcc_generated_files/system.h"
#include <stdio.h>
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"
#include "mcc_generated_files/rtcc.h"
#include "state.h"
#include "ESP8266_handler.h"
#include "mcc_generated_files/memory/flash.h"

//void setRTCCtimeFromAPI(char  uart_buffer_ptr[], int sizeOfBuffer);

char uart_buffer[1024*2+1];
int uart_buffer_size = 0;
    int uart_buffer_index = 0;    

void uart_interrupt_handler();
void (*uart_interrupt_handler_ptr)(void) = uart_interrupt_handler;

int main(void)
{   
    // initialize the device
    SYSTEM_Initialize();    
    UART1_SetRxInterruptHandler(uart_interrupt_handler_ptr);
    setRTCCtimeFromAPI(uart_buffer, sizeof(uart_buffer)); 
    initStatusLED();    
    
    int state;
    if(!connected(uart_buffer, sizeof(uart_buffer))){
        state = NO_WIFI;
    }
    
    //setStatus(ACTIVE);
    while (1)
    {
        switch(state){
            case NO_WIFI:
                setStatus(NO_WIFI);
                break;
        }
    }

    return 1;
}

void uart_interrupt_handler(){
    while(UART1_IsRxReady()){                
        uint8_t data = UART1_Read();
        printf("%c", data);
        uart_buffer[uart_buffer_index] = data;
        uart_buffer_index++;
    }
}

void clearUartBuffer(){
    
}


