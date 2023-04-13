/*
 * File:   statusLED.c
 * Author: Joect
 *
 * Created on April 10, 2023, 8:23 PM
 */


#include <xc.h>
#include "state.h"

void turnOnRedLED();
void turnOnYellowLED();
void turnOnGreenLED();
void turnOnBlueLED();
void turnOffRedLED();
void turnOffYellowLED();
void turnOffGreenLED();
void turnOffBlueLED();

//enum Status {ACTIVE, NOT_ACTIVE, ALARM, NO_ALARM_PERIOD, NO_WIFI, NOT_INITIALIZED, NOT_CONNECTED_TO_DATESERVER};
//State currentState = NOT_INITIALIZED;

//int state = NOT_INITIALIZED;

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

void setStatus(int state){
    
    turnOffRedLED();
    turnOffYellowLED();
    turnOffGreenLED();
    turnOffBlueLED();
       
    switch(state){
        case ACTIVE: 
             turnOnGreenLED();
             break;
        case NOT_ACTIVE: 
            turnOnYellowLED();
            break;
        case NO_WIFI:
             turnOnBlueLED();
             turnOnRedLED();          
             break;
        case NOT_INITIALIZED:
            turnOnRedLED();
            break;
        default: 
            initStatusLED();
            break;
    }    
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

