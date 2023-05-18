/*
 * File:   statusLED.c
 * Author: Joect
 *
 * Created on April 10, 2023, 8:23 PM
 */


#include <xc.h>
#include "stateManager.h"
#include "flashInterface.h"
#include "LED_handler.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/tmr2.h"
#include "mcc_generated_files/tmr3.h"
#include "interrupt_handlers.h"
#include "PIR_sensor_handler.h"
#include "mcc_generated_files/ext_int.h"
#include "mcc_generated_files/delay.h"

void enterIdleMode();


int globalState;

// Help function

void setState(int state){
  
    turnOffRedLED();
    turnOffYellowLED();
    turnOffGreenLED();
    turnOffBlueLED();
       
    switch(state){
        case ALARM:
            globalState = ALARM;
            break;
        case ACTIVE: 
            globalState = ACTIVE;
            break;
        case NOT_ACTIVE: 
            turnOnYellowLED();
            globalState = NOT_ACTIVE;
            break;
        case NO_WIFI:
            turnOnBlueLED();
            turnOnRedLED();
            globalState = NO_WIFI;
            break;
        case CONNECTED_TO_WIFI:
            globalState = CONNECTED_TO_WIFI;
            turnOnBlueLED();
            turnOnGreenLED();
            break;
        case CONNECTED_TO_SOCKET:
            globalState = CONNECTED_TO_SOCKET;
            break;
        case NOT_CONNECTED_TO_SOCKET:
            turnOnBlueLED();
            turnOnYellowLED();
            globalState = NOT_CONNECTED_TO_SOCKET;
            break;
        case LOST_CONNECTION: 
            globalState = LOST_CONNECTION;
            break;        
        case NOT_INITIALIZED:
            turnOnRedLED();
            turnOnYellowLED();
            turnOnBlueLED();
            turnOnGreenLED();
            globalState = NOT_INITIALIZED;
            break;
        case RE_INIT: 
            turnOnYellowLED();
            globalState = RE_INIT;
            break;
        default: 
            initStatusLED();
            break;
    } 
    return;
}

bool alarmActive(){
    struct tm currentTime;    
    RTCC_TimeGet(&currentTime);   
//    int * alarmTime = getStoredAlarmPeriod();
    int start = getStartTime();
    int end = getEndTime();
    int currentTimeConverted = 100*currentTime.tm_hour + currentTime.tm_min;
    if(start > end){
        if( (currentTimeConverted >= start && currentTimeConverted >= end) || (currentTimeConverted <= start && currentTimeConverted <= end) ){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(currentTimeConverted > start && currentTimeConverted < end){
            return true;
        }
        else{
            return false; 
        }
    }
}

int getState(){
    return globalState;
}

// State functions

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
    turnPIROn();
    //TMR3_Start();
    EX_INT1_InterruptEnable();
        enterIdleMode();
        handleIncommingMessage();  
    EX_INT1_InterruptDisable();
    //TMR3_Stop();
    turnPIROff();
    return;
}

void not_active_state(){
        //TMR3_Start();
        enterIdleMode();
        handleIncommingMessage();
        //TMR3_Stop();        
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

