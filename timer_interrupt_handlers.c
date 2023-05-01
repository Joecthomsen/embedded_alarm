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

int counterActive = 0;
int counterLostConnection = 0;


//run every 4 second
void timer1_interrupt_handler() {
    
    if(getState() == NOT_CONNECTED_TO_SOCKET){
        turnOnGreenLED();
        DELAY_milliseconds(50);
        turnOffGreenLED();
        hardResetWifiModule(); 
        if(connectToSocket()){
            State currentState = CONNECTED_TO_SOCKET;
            setState(currentState);
        }
    }
    else if(getState() == NO_WIFI){
        turnOnGreenLED();
        DELAY_milliseconds(50);
        turnOffGreenLED();
        hardResetWifiModule(); 
        if(connectedToWiFi()){
            State state = CONNECTED_TO_WIFI;
            setState(CONNECTED_TO_WIFI);
        }
    }
    return;
}

// Run every 2 second
void timer2_interrupt_handler(){
    
    switch(getState()){
        case ACTIVE: {
            turnOnGreenLED();
            DELAY_milliseconds(100);
            turnOffGreenLED();
            if(counterActive == 15){
                if(!connectedToWiFi()){
                    State currentState = LOST_CONNECTION;
                    setState(currentState);
                }
                counterActive = -1;
            }
            counterActive++;
        }
        break;
        case LOST_CONNECTION: {
            turnOnYellowLED();
            DELAY_milliseconds(100);
            turnOffYellowLED();
            if(!connectedToWiFi()){
                if(counterLostConnection == 2){
                    hardResetWifiModule();
                    counterLostConnection = 0;
                }
                else{
                    counterLostConnection++;
                }
            }
            else{
                counterLostConnection = 0;
                State currentState = ACTIVE;
                setState(currentState);
            }
        }
        break;
    }
}

//void timer3_interrupt_handler(){
//    syncAlarmPeriodFromServer();
//    turnOnBlueLED();
//    DELAY_milliseconds(50);
//    turnOffBlueLED();
//    if(alarmActive() && getState() == NOT_ACTIVE){
//        State state = ACTIVE;
//        setState(state);
//    }
//    else if(!alarmActive() && getState() == ACTIVE){
//        State state = NOT_ACTIVE;
//        setState(state);
//    }
//    return;
//}










