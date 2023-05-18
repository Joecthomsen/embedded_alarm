 

#include "mcc_generated_files/system.h"
#include <stdio.h>
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"
#include "mcc_generated_files/rtcc.h"
#include "stateManager.h"
#include "ESP8266_handler.h"
#include "mcc_generated_files/memory/flash.h"
#include "uart_handler.h"
#include "flashInterface.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/tmr2.h"
#include "mcc_generated_files/tmr3.h"
#include "interrupt_handlers.h"
#include "LED_handler.h"
#include "mcc_generated_files/ext_int.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/delay.h"
#include "PIR_sensor_handler.h"
#include <string.h>


void uart_interrupt_handler();

void init();
void initInerrupts();
void enterIdleMode();
void handleIncommingMessage();

int main(void)
{   
    SYSTEM_Initialize(); 
    /* This is code pseudo for pseudo setting deviceID as the ID is getting deleted every time the PIC24 is reflashed.
     * 
     * This code can be thinked aboute similar to what an application, NFC or bluetooth
     * has to do, in order for the endnode to establish connection to the right owner
     * 
     * THIS CODE HAS TO BE DELETED IN PRODUCTION
     */
    //Start of pseudo init code
    FLASH_Unlock(FLASH_UNLOCK_KEY);
        FLASH_WriteWord16(DEVICE_ID_ADDRESS, 0x1);  //Write the number "1" as device id. 
    FLASH_Lock();
    setAlarmPeriod(1200, 1738);
    //End of pseudo init code
   
    init();
    
    while (1)
    {
        int state = getState();
        switch (state){
            case ALARM: 
                alarmState();
                break;
            case ACTIVE:
                activeState();
                break;
            case NOT_ACTIVE:
                not_active_state();
                break;
            case CONNECTED_TO_WIFI:
                init();
                break;
            case CONNECTED_TO_SOCKET:
                init();
                break;
            case NOT_CONNECTED_TO_SOCKET:
                not_connected_to_socket_state();
                break;
            case RE_INIT:              
                init();
                break;
            case NO_WIFI:
                no_wifi_state();
                break;
            case NO_ALARM_PERIOD:
                _LATB12 = 1;
                while(1){;}
                break;
            case LOST_CONNECTION: {
                lost_connection_state();
                break;
            }
        }
    }
    return 1;
}

void init(){
    //Only do the first init if not already connected to socket.
    if(getState() != CONNECTED_TO_SOCKET){
        if(getState() != CONNECTED_TO_WIFI){
            initInerrupts();
            initStatusLED();
            turnOnYellowLED();
            initESP8266();
            if(!deviceHasId()){
                State currentState = NOT_INITIALIZED;
                setState(currentState);
                return;
            }
            if(!connectedToWiFi()){            
                State currentState = NO_WIFI;
                setState(currentState);
                return;
            }
        }
        if(!connectToSocket()){
            State state = NOT_CONNECTED_TO_SOCKET;
            setState(state);
            return;
        }
    }
    if(!registerDevice()){
        State state = NOT_REGISTERED_WITH_SERVER;
        setState(state);
        return;
    }
    setRTCCtimeFromServer();   //TODO make bool to check that everything went as expected
    
    if(alarmActive()){
        State currentState = ACTIVE; 
        setState(currentState);
        //TMR3_Start();
    }
    else if(!alarmActive()){
        State currentState = NOT_ACTIVE; 
        setState(currentState);
        TMR3_Start();
    }
    initPIR();
    turnPIROff();
    T2CONbits.TSIDL = 0;    //continue timer operation during idle mode. 
    DSCONbits.DSEN = 0; // Set the power saving mode to "Normal Sleep on execution of PWRSAV #0"
    //turnOffYellowLED();
    return;
}

void initInerrupts(){
    
    //Turn off sensor interrupt until the system is in the alarm state
    EX_INT1_InterruptDisable();    
    //Function pointers for ISR
    void (*uart_interrupt_handler_ptr)(void) = uart_interrupt_handler;
    void (*timer1_NO_WIFI_interrupt_handler_ptr)(void) = timer1_interrupt_handler; 
//    void (*timer2_active_state_handler_ptr)(void) = timer2_interrupt_handler;
    void (*timer3_active_state_handler_ptr)(void) = timer3_interrupt_handler;
        
    //Create ISR callback
    UART1_SetRxInterruptHandler(uart_interrupt_handler_ptr);
    TMR1_SetInterruptHandler(timer1_NO_WIFI_interrupt_handler_ptr);
    TMR1_Stop();
//    TMR2_SetInterruptHandler(timer2_active_state_handler_ptr);
    //TMR2_Stop();
    TMR3_SetInterruptHandler(timer3_active_state_handler_ptr);
    TMR3_Stop();
}
