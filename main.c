
#include "mcc_generated_files/system.h"
#include <stdio.h>
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"
#include "mcc_generated_files/rtcc.h"
#include "state.h"
#include "ESP8266_handler.h"
#include "mcc_generated_files/memory/flash.h"
#include "uart_buffer.h"
#include "flashInterface.h"
#include "mcc_generated_files/tmr1.h"
#include "timer_interrupt_handlers.h"

void uart_interrupt_handler();

void init();
void initInerrupts();
void no_wifi_state();

int main(void)
{   
    SYSTEM_Initialize(); 
    /* This is code pseudo for pseudo setting deviceID as the ID is getting deleted every time the PIC24 is reflashed.
     * This is to be deleted in production
     */
    //Start of pseudo init code
    FLASH_Unlock(FLASH_UNLOCK_KEY);
        FLASH_WriteWord16(DEVICE_ID_ADDRESS, 0x1);
    FLASH_Lock();
    //End of pseudo init code
   
    initInerrupts();
    init();

    
     
    while (1)
    {
        int state = getState();
        switch (state){
            case ACTIVE: 
                break;
            case RE_INIT:
                _LATB13 = 1;
                init();
                break;
            case NO_WIFI:
                no_wifi_state();
                break;    
        }
    }

    return 1;
}

void init(){
    
 
    initStatusLED();
    initESP8266();
    if(!deviceRegistered()){
        State currentState = NOT_INITIALIZED;
        setStatus(currentState);
        return;
    }
    if(!connected()){
        State currentState = NO_WIFI;
        setStatus(currentState);
        return;
    }

    setRTCCtimeFromAPI();  
    getPeriod();
    State currentState = ACTIVE; 
    setStatus(currentState);
    return;
}

void initInerrupts(){
    
    void (*uart_interrupt_handler_ptr)(void) = uart_interrupt_handler;
    void (*timer1_NO_WIFI_interrupt_handler_ptr)(void) = timer1_NO_WIFI_interrupt_handler;    
    UART1_SetRxInterruptHandler(uart_interrupt_handler_ptr);
    TMR1_SetInterruptHandler(timer1_NO_WIFI_interrupt_handler_ptr);
    TMR1_Stop();
}


void no_wifi_state(){
    TMR1_Start();
    while(getState() == NO_WIFI){
        ;
    }
    TMR1_Stop();
    return;
}
