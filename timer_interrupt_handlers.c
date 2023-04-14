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


void timer1_NO_WIFI_interrupt_handler() {
    
    _LATB12 = ~ _LATB12;
    
 /*   
    int k = 0;
    if(connected()){
        //TMR1_Stop();
        State currentState = RE_INIT;
        setStatus(currentState);
    }
  * */
    //return;
}


