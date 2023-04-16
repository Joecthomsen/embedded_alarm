/*
 * File:   settings.c
 * Author: Joect
 *
 * Created on April 15, 2023, 4:34 PM
 */


#include <xc.h>
#include "settings.h"

int period[2];

int * getStoredAlarmPeriod(){
    return period;
}

void setAlarmPeriod(int startTime, int endTime){
    period[0] = startTime;
    period[1] = endTime;

}

