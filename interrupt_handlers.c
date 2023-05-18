/*
 * File:   timer_interrupt_handlers.c
 * Author: Joect
 *
 * Created on April 13, 2023, 7:09 PM
 */

#include <xc.h>
#include "ESP8266_handler.h"
#include "stateManager.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/delay.h"

#define CONECTION_CHECK_SECONDS 30

int counter_NO_SOCKET_state = 0;


//Check if alarm state has changed every minute. 
//This ISR has to be called from inside the callback() in the rctt.c file 
void RTCC_InterrurpHandler(){
    
    int currentState = getState();
    
    if(currentState == NOT_ACTIVE || currentState == ACTIVE){
        turnOnBlueLED();
        DELAY_milliseconds(50);
        turnOffBlueLED();
        if(alarmActive() && currentState == NOT_ACTIVE){
            State state = ACTIVE;
            setState(state);
        }
        else if(!alarmActive() && currentState == ACTIVE){
            State state = NOT_ACTIVE;
            setState(state);
        }
    }
}


//run every 4 second
void timer1_interrupt_handler() {
    
    int state = getState();
    
//    if(state == NOT_CONNECTED_TO_SOCKET){
//        turnOnGreenLED();
//        DELAY_milliseconds(50);
//        turnOffGreenLED();
//        if(connectToSocket()){
//            State currentState = CONNECTED_TO_SOCKET;
//            setState(currentState);
//        }
//
//        else if(!connectedToWiFi()){
//            State state = NO_WIFI;
//            setState(state);
//        }
//        hardResetWifiModule();
//        State state = NO_WIFI;
//        setState(state);
//    }
//    else if(state == NO_WIFI){
        turnOnGreenLED();
        DELAY_milliseconds(50);
        turnOffGreenLED();
        hardResetWifiModule(); 
        DELAY_milliseconds(50);
        if(connectedToWiFi()){
            State state = CONNECTED_TO_WIFI;
            setState(state);
            if(connectToSocket()){
                State state = CONNECTED_TO_SOCKET;
                setState(state);
            }
        }
        else{
            State state = NO_WIFI;
            setState(state);
        }
//    }
//    int k = 0;
    return;
}

void timer3_interrupt_handler(){
    turnOnBlueLED();
    DELAY_milliseconds(50);
    turnOffBlueLED();
//    if(!getSocketStatus()){
//        int k = 0;
//    }
}










