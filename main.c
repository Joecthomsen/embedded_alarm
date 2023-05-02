
#include "mcc_generated_files/system.h"
#include <stdio.h>
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/uart2.h"
#include "mcc_generated_files/rtcc.h"
#include "stateManager.h"
#include "ESP8266_handler.h"
#include "mcc_generated_files/memory/flash.h"
#include "uart_buffer.h"
#include "flashInterface.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/tmr2.h"
#include "mcc_generated_files/tmr3.h"
#include "timer_interrupt_handlers.h"
#include "mcc_generated_files/ext_int.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/delay.h"
#include <string.h>


void uart_interrupt_handler();

void init();
void initInerrupts();
void enterIdleMode();
void handleIncommingMessage();

//States
void alarmState();
void no_wifi_state();
void activeState();
void lost_connection_state();
void not_active_state();
void not_connected_to_socket_state();

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

void not_connected_to_socket_state(){
    TMR1_Start();
    while(getState() == NOT_CONNECTED_TO_SOCKET){
        enterIdleMode();
    }
    TMR1_Stop();
   
}

void alarmState(){    
    turnOnRedLED();
    alarmTriggered();
    //setAlarm();
    //INTERRUPT_GlobalDisable();
    DELAY_milliseconds(10000);  //Change to some clear condition from app.
    //enterIdleMode();
    turnOffRedLED();
    State state = ACTIVE;
    setState(state);
}

void activeState(){
    turnOnGreenLED();
    TMR3_Start();
    EX_INT1_InterruptEnable();
        enterIdleMode();
        handleIncommingMessage();  
    EX_INT1_InterruptDisable();
    TMR3_Stop();
    return;
}

void not_active_state(){
        TMR3_Start();
        enterIdleMode();
        handleIncommingMessage();
        TMR3_Stop();        
}

void no_wifi_state(){
    TMR1_Start();
    while(getState() == NO_WIFI){
        enterIdleMode();
    }
    TMR1_Stop();
    return;
}

void lost_connection_state(){
    //TMR2_Start();
    while(getState() == LOST_CONNECTION){
        enterIdleMode();
    }
    //TMR2_Stop();
}

void enterIdleMode(){
    __asm__(
        "PWRSAV #1\n"   //1=idle ; 0=sleep
    );
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

