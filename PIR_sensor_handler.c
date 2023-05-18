/*
 * File:   PIR_sensor_handler.c
 * Author: johannes
 *
 * Created on May 5, 2023, 10:02 PM
 */


#include <xc.h>

void initPIR(){
    _TRISA2 = 0;
}
void turnPIROn(){
    _LATA2 = 1;
}
void turnPIROff(){
    _LATA2 = 0;
}