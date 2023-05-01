/*
 * File:   deviceConfiguration.c
 * Author: Joect
 *
 * Created on April 12, 2023, 7:49 PM
 */


#include <xc.h>
#include <stdbool.h>
#include "definitions.h"
#include "mcc_generated_files/memory/flash.h"

bool deviceHasId(){
    if(FLASH_ReadWord16(DEVICE_ID_ADDRESS) == 0xFFFF){
        return false;
    }
    return true;
}

uint16_t getUserId(){
    return FLASH_ReadWord16(DEVICE_ID_ADDRESS);
}

uint16_t getStartTime(){
    return FLASH_ReadWord16(START_ALARM_TIME);
}
uint16_t getEndTime(){
    return FLASH_ReadWord16(END_ALARM_TIME);
}

void setAlarmPeriod(int startTime, int endTime){
    
    uint16_t id = getUserId();
    
    FLASH_Unlock(FLASH_UNLOCK_KEY);
        uint32_t addreseOfPageToErrase = FLASH_GetErasePageAddress(START_ALARM_TIME);    
        FLASH_ErasePage(addreseOfPageToErrase);
        FLASH_WriteWord16(DEVICE_ID_ADDRESS, id);
        FLASH_WriteWord16(START_ALARM_TIME, startTime);  //Write the number "1" as device id. 
        FLASH_WriteWord16(END_ALARM_TIME, endTime);  //Write the number "1" as device id. 
    FLASH_Lock();
}

