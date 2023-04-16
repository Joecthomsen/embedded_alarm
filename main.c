
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


void uart_interrupt_handler();

void init();
void initInerrupts();

//States
void alarmState();
void no_wifi_state();
void activeState();
void lost_connection_state();
void not_active_state();

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
            case ALARM: 
                alarmState();
            case ACTIVE:
                activeState();
                break;
            case NOT_ACTIVE:
                not_active_state();
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
    initStatusLED();
    initESP8266();
    if(!deviceRegistered()){
        State currentState = NOT_INITIALIZED;
        setState(currentState);
        return;
    }
    if(!connected()){
        State currentState = NO_WIFI;
        setState(currentState);
        return;
    }

    setRTCCtimeFromAPI();
    if(!setAlarmPeriodFromServer()){
        State currentState = NO_ALARM_PERIOD;
        setState(currentState);
        return;
    }
    if(alarmActive()){
        State currentState = ACTIVE; 
        setState(currentState);
        TMR3_Start();
    }
    else{
        State currentState = NOT_ACTIVE; 
        setState(currentState);
        TMR3_Start();
    }
    return;
}

void initInerrupts(){
    
    //Turn off sensor interrupt until the system is in the alarm state
    //EX_INT1_InterruptDisable();
    
    //Function pointers for ISR
    void (*uart_interrupt_handler_ptr)(void) = uart_interrupt_handler;
    void (*timer1_NO_WIFI_interrupt_handler_ptr)(void) = timer1_interrupt_handler; 
    void (*timer2_active_state_handler_ptr)(void) = timer2_interrupt_handler;
    void (*timer3_active_state_handler_ptr)(void) = timer3_interrupt_handler;
        
    //Create ISR callback
    UART1_SetRxInterruptHandler(uart_interrupt_handler_ptr);
    TMR1_SetInterruptHandler(timer1_NO_WIFI_interrupt_handler_ptr);
    TMR1_Stop();
    TMR2_SetInterruptHandler(timer2_active_state_handler_ptr);
    TMR2_Stop();
    TMR3_SetInterruptHandler(timer3_active_state_handler_ptr);
    TMR3_Stop();
}

void alarmState(){
    //INTERRUPT_GlobalDisable();
    //turnOffGreenLED();
    //turnOffYellowLED();
    //turnOffBlueLED();
    turnOnRedLED();
    DELAY_milliseconds(10000);  //Change to some clear condition from app.
    //INTERRUPT_GlobalEnable();
    turnOffRedLED();
    State state = ACTIVE;
    setState(state);
}

void activeState(){
    EX_INT1_InterruptEnable();
    TMR2_Start();
    while(getState() == ACTIVE){;}
    EX_INT1_InterruptDisable();
    TMR2_Stop();
    return;
}

void no_wifi_state(){
    TMR1_Start();
    while(getState() == NO_WIFI){;}
    TMR1_Stop();
    return;
}

void lost_connection_state(){
    TMR2_Start();
    while(getState() == LOST_CONNECTION){;}
    TMR2_Stop();
}

void not_active_state(){
    while(getState() == NOT_ACTIVE){;}
}