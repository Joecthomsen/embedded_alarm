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


