/*
 * File:   statusLED.c
 * Author: Joect
 *
 * Created on April 10, 2023, 8:23 PM
 */


#include <xc.h>
#include "stateManager.h"
#include "flashInterface.h"

void turnOnRedLED();
void turnOnYellowLED();
void turnOnGreenLED();
void turnOnBlueLED();
void turnOffRedLED();
void turnOffYellowLED();
void turnOffGreenLED();
void turnOffBlueLED();

int globalState;

void initStatusLED(){
    
    
    _TRISB13 = 0;
    _TRISB12 = 0;
    _TRISB11 = 0;
    _TRISB10 = 0;
    
    turnOffRedLED();
    turnOffYellowLED();
    turnOffGreenLED();
    turnOffBlueLED();
    
}

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
    int k = 0;
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

void turnOnRedLED(){
    _LATB11 = 1;
}
void turnOnYellowLED(){
    _LATB12 = 1;
}
void turnOnGreenLED(){
    _LATB13 = 1;
}
void turnOnBlueLED(){
    _LATB10 = 1;
}
void turnOffRedLED(){
    _LATB11 = 0;
}
void turnOffYellowLED(){
    _LATB12 = 0;
}
void turnOffGreenLED(){
    _LATB13 = 0;
}
void turnOffBlueLED(){
    _LATB10 = 0;
}

