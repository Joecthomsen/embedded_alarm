
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

//void setRTCCtimeFromAPI(char  uart_buffer_ptr[], int sizeOfBuffer);


void uart_interrupt_handler();
void (*uart_interrupt_handler_ptr)(void) = uart_interrupt_handler;
void mainLoop();

int main(void)
{   
    /* This is code pseudo for pseudo setting deviceID as the ID is getting deleted every time the PIC24 is reflashed.
     * This is to be deleted in production
     */
    //Start of pseudo init code
    FLASH_Unlock(FLASH_UNLOCK_KEY);
        FLASH_WriteWord16(DEVICE_ID_ADDRESS, 0x1);
    FLASH_Lock();
    //End of pseudo init code
    
    // initialize the device
    SYSTEM_Initialize();    
    UART1_SetRxInterruptHandler(uart_interrupt_handler_ptr);
    initStatusLED();
    bool devideReg = deviceRegistered();
    int t = 0;

    if(!deviceRegistered()){
        State currentState = NOT_INITIALIZED;
        setStatus(currentState);
        mainLoop();
    }
    if(!connected()){
        State currentState = NO_WIFI;
        setStatus(currentState);
        mainLoop();
    }
        
    setRTCCtimeFromAPI();   
    
    getPeriod();
    

    //= getAlarmPeriod();
    /*
    if(!connected(uart_buffer, sizeof(uart_buffer))){
        state = NO_WIFI;
    }
    */
    //setStatus(ACTIVE);


    return 1;
}

void mainLoop(){

    while (1)
    {

    }
}


