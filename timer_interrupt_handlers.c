/*
 * File:   timer_interrupt_handlers.c
 * Author: Joect
 *
 * Created on April 13, 2023, 7:09 PM
 */

#include <xc.h>
#include "ESP8266_handler.h"
#include "state.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/delay.h"

#define CONECTION_CHECK_SECONDS 30

int activeCounter = 0;


void timer1_NO_WIFI_interrupt_handler() {   
    turnOnYellowLED();
    DELAY_milliseconds(50);
    turnOffYellowLED();
    hardResetWifiModule(); 
    if(connected()){
        State currentState = RE_INIT;
        setStatus(currentState);
    }
    return;
}

// Run every second second
void timer2_active_state_handler(){
    turnOnGreenLED();
    DELAY_milliseconds(100);
    turnOffGreenLED();
    if(activeCounter == CONECTION_CHECK_SECONDS/2){
        if(!connected()){
            State currentState = LOST_CONNECTION;
            setStatus(currentState);
            return;
        }
        activeCounter = -1;
    }
    activeCounter++;
}


