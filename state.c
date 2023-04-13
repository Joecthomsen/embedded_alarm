/*
 * File:   statusLED.c
 * Author: Joect
 *
 * Created on April 10, 2023, 8:23 PM
 */


#include <xc.h>
#include "state.h"

void initStatusLED(){
    _TRISB11 = 0;
    _TRISB12 = 0;
    _TRISB13 = 0;
    _LATB11 = 0;
    _LATB12 = 0;
    _LATB13 = 0;    
}

void setStatus(int status){
    
    _LATB11 = 0;
    _LATB12 = 0;
    _LATB13 = 0;  
    
    switch(status){
        case ACTIVE: _LATB13 = 1;
        break;
        case NOT_ACTIVE: _LATB12 = 1;
        break;
        case ALARM: _LATB11 = 1;
        break;
        case NO_WIFI: 
            _LATB11 = 1;
            _LATB12 = 1;
        break;
        default: initStatusLED();
        break;
    }
    
}
//enum status getStatus(){}

